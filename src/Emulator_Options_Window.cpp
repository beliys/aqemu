/****************************************************************************
**
** Copyright (C) 2008-2010 Andrey Rijov <ANDron142@yandex.ru>
** Copyright (C) 2016 Tobias Gläßer
**
** This file is part of AQEMU.
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor,
** Boston, MA  02110-1301, USA.
**
****************************************************************************/

#include <QDir>
#include <QFileDialog>
#include <QHeaderView>

#include "Utils.h"
#include "System_Info.h"
#include "Emulator_Options_Window.h"

Emulator_Options_Window::Emulator_Options_Window( QWidget *parent )
	: QDialog( parent )
{
	ui.setupUi( this );

	ui.Table_Systems->resizeColumnsToContents();
	ui.Table_Systems->resizeRowsToContents();

	QHeaderView *hv = new QHeaderView( Qt::Vertical, ui.Table_Systems );
	hv->setSectionResizeMode( QHeaderView::Fixed );
	ui.Table_Systems->setVerticalHeader( hv );

	hv = new QHeaderView( Qt::Horizontal, ui.Table_Systems );
	hv->setSectionResizeMode( QHeaderView::Stretch );
	ui.Table_Systems->setHorizontalHeader( hv );

	Update_Info = false;

	#ifdef Q_OS_WIN32
	ui.RB_KVM->setEnabled( false );
	#endif
}

void Emulator_Options_Window::on_Button_Find_clicked()
{
	if( ui.Edit_Path_to_Dir->text().isEmpty() )
	{
		AQGraphic_Warning( tr("Error!"), tr("Path is Empty!") );
		return;
	}

	QDir dir = QDir( ui.Edit_Path_to_Dir->text() );
	if( ! dir.exists() )
	{
		AQGraphic_Warning( tr("Warning"), tr("Path doesn't exist!") );
		return;
	}

	// Set Type and Bin files
	QMap<QString, QString> list;

	list = System_Info::Find_QEMU_Binary_Files( ui.Edit_Path_to_Dir->text() );

	// Clear old bin files
	ui.Table_Systems->clearContents();
	while( ui.Table_Systems->rowCount() > 0 ) ui.Table_Systems->removeRow( 0 );

	// Add bin files to Table_Systems
	QMap<QString, QString>::const_iterator iter = list.constBegin();
	while( iter != list.constEnd() )
	{
		ui.Table_Systems->insertRow( ui.Table_Systems->rowCount() );

		QTableWidgetItem *newItem = new QTableWidgetItem( iter.key() );
		ui.Table_Systems->setItem( ui.Table_Systems->rowCount()-1, 0, newItem );

		newItem = new QTableWidgetItem( QDir::toNativeSeparators(iter.value()) );
		ui.Table_Systems->setItem( ui.Table_Systems->rowCount()-1, 1, newItem );

		++iter; // next value
	}

	// Update emulator info
	Update_Info = true;
	Update_Emulator();
	Update_Info = false;
}

void Emulator_Options_Window::on_TB_Browse_clicked()
{
	QString dir = QFileDialog::getExistingDirectory( this, tr("Select Directory"),
													 Get_Last_Dir_Path(ui.Edit_Path_to_Dir->text()),
													 QFileDialog::ShowDirsOnly );

	if( ! dir.isEmpty() )
		ui.Edit_Path_to_Dir->setText( QDir::toNativeSeparators(dir) );
}

void Emulator_Options_Window::done(int r)
{
    if ( r == QDialog::Accepted )
    {
	    // Find all Emulators Names
	    if( ui.Edit_Name->text().isEmpty() )
	    {
		    AQGraphic_Warning( tr("Error!"), tr("Enulator Name is Empty!") );
		    return;
	    }

	    if( ! Name_Valid(ui.Edit_Name->text()) )
	    {
		    AQGraphic_Warning( tr("Error!"), tr("This emulator name is already used!") );
		    return;
	    }

	    // Name
	    Current_Emulator.Set_Name( ui.Edit_Name->text() );

	    // Path
	    Current_Emulator.Set_Path( ui.Edit_Path_to_Dir->text() );

	    // Save Binary List
	    QMap<QString, QString> bin_files;
	    for( int ix = 0; ix < ui.Table_Systems->rowCount(); ix++ )
	    {
		    bin_files[ ui.Table_Systems->item(ix, 0)->text() ] = ui.Table_Systems->item(ix, 1)->text();
	    }
	    Current_Emulator.Set_Binary_Files( bin_files );

	    // Check version
	    if( ui.RB_Check_Version->isChecked() ) Current_Emulator.Set_Check_Version( true );
	    else Current_Emulator.Set_Check_Version( false );

	    // Check available options
	    if( ui.RB_Check_Options->isChecked() ) Current_Emulator.Set_Check_Available_Options( true );
	    else Current_Emulator.Set_Check_Available_Options( false );

	    // Force version
	    if( ui.RB_Force_Version->isChecked() ) Current_Emulator.Set_Force_Version( true );
	    else Current_Emulator.Set_Force_Version( false );

	    // Version
	    Current_Emulator.Set_Version( String_To_Emulator_Version(ui.CB_Version->currentText()) );

	    // Update emulator?
	    Update_Emulator();
    }
    QDialog::done(r);
}

Emulator Emulator_Options_Window::Get_Emulator() const
{
	return Current_Emulator;
}

void Emulator_Options_Window::Set_Emulator( const Emulator &emul )
{
	Current_Emulator = emul;

	// Name
	ui.Edit_Name->setText( Current_Emulator.Get_Name() );

	// Path
	ui.Edit_Path_to_Dir->setText( Current_Emulator.Get_Path() );

	// Version
	if( Current_Emulator.Get_Check_Version() ) ui.RB_Check_Version->setChecked( true ); // Check version
	else if( Current_Emulator.Get_Check_Available_Options() ) ui.RB_Check_Options->setChecked( true ); // Check available options
	else if( Current_Emulator.Get_Force_Version() ) ui.RB_Force_Version->setChecked( true ); // Force version
	else ui.RB_Save_Options->setChecked( true ); // Use saved emulator available options

	// Force Version
	switch( Current_Emulator.Get_Version() )
	{
		case VM::QEMU_2_0:
			ui.CB_Version->setCurrentIndex( 1 );
			break;

		default:
			AQWarning( "void Emulator_Options_Window::Set_Emulator( Emulator emul )",
					   "Version NOT Valid! Use Default" );
			ui.CB_Version->setCurrentIndex( 0 );
			break;
	}

	// Emulator Binary Files
	QMap<QString, QString> bin_files = Current_Emulator.Get_Binary_Files();

	// Clear
	ui.Table_Systems->clearContents();
	while( ui.Table_Systems->rowCount() > 0 ) ui.Table_Systems->removeRow( 0 );

	// Add new
	for( QMap<QString, QString>::const_iterator iter = bin_files.constBegin(); iter != bin_files.constEnd(); iter++ )
	{
		ui.Table_Systems->insertRow( ui.Table_Systems->rowCount() );

		QTableWidgetItem *newItem;
		/*if( ui.RB_QEMU->isChecked() )
		{
			newItem = new QTableWidgetItem( iter.key() );
			ui.Table_Systems->setItem( ui.Table_Systems->rowCount()-1, 0, newItem );
		}
		else
		{
			newItem = new QTableWidgetItem( iter.key() );
			ui.Table_Systems->setItem( ui.Table_Systems->rowCount()-1, 0, newItem );
		}*/

		newItem = new QTableWidgetItem( iter.value() );
		ui.Table_Systems->setItem( ui.Table_Systems->rowCount()-1, 1, newItem );
	}

	Update_Info = false; // Update emulator info
}

void Emulator_Options_Window::Set_All_Emulators_Names( const QStringList &allNames )
{
	All_Emulators_Names = allNames;
}

void Emulator_Options_Window::on_Edit_Name_textChanged()
{
	if( Name_Valid(ui.Edit_Name->text()) )
		ui.Label_Name_Valid->setPixmap( QPixmap(":/ok.png") );
	else
		ui.Label_Name_Valid->setPixmap( QPixmap(":/remove.png") );
}

void Emulator_Options_Window::on_Edit_Path_to_Dir_textChanged()
{
	if( QFile::exists(ui.Edit_Path_to_Dir->text()) )
		ui.Button_Find->setEnabled( true );
	else
		ui.Button_Find->setEnabled( false );

	Update_Info = true; // Update emulator info
}

void Emulator_Options_Window::on_Table_Systems_itemDoubleClicked( QTableWidgetItem *item )
{
	QString binName = QFileDialog::getOpenFileName( this, tr("Select QEMU Binary File"),
													Get_Last_Dir_Path(ui.Edit_Path_to_Dir->text()),
													tr("All Files (*)") );

	if( ! binName.isEmpty() )
	{
		if( ui.Table_Systems->currentRow() < 0 ) return;

		ui.Table_Systems->item( ui.Table_Systems->currentRow(), 1 )->setText( QDir::toNativeSeparators(binName) );

		Update_Info = true; // Update emulator info
	}
}

void Emulator_Options_Window::on_RB_QEMU_toggled( bool checked )
{
	// Clear
	ui.Table_Systems->clearContents();
	while( ui.Table_Systems->rowCount() > 0 ) ui.Table_Systems->removeRow( 0 );

	// Add new items
	QMap<QString, QString> bin_files = System_Info::Find_QEMU_Binary_Files( "" );

	for( QMap<QString, QString>::const_iterator iter = bin_files.constBegin(); iter != bin_files.constEnd(); iter++ )
	{
		ui.Table_Systems->insertRow( ui.Table_Systems->rowCount() );

		QTableWidgetItem *newItem = new QTableWidgetItem( iter.key() );
		ui.Table_Systems->setItem( ui.Table_Systems->rowCount()-1, 0, newItem );
	}

	Update_Info = true; // Update emulator info
}


void Emulator_Options_Window::on_RB_KVM_toggled( bool checked )
{
	/*// Clear
	ui.Table_Systems->clearContents();
	while( ui.Table_Systems->rowCount() > 0 ) ui.Table_Systems->removeRow( 0 );

	// Add new items
	QMap<QString, QString> bin_files = System_Info::Find_KVM_Binary_Files( "" );

	for( QMap<QString, QString>::const_iterator iter = bin_files.constBegin(); iter != bin_files.constEnd(); iter++ )
	{
		ui.Table_Systems->insertRow( ui.Table_Systems->rowCount() );

		QTableWidgetItem *newItem = new QTableWidgetItem( iter.key() );
		ui.Table_Systems->setItem( ui.Table_Systems->rowCount()-1, 0, newItem );
	}

	Update_Info = true; // Update emulator info*/ //tobgle //FIXME?!
}

bool Emulator_Options_Window::Name_Valid( const QString &name )
{
	if( name.isEmpty() ) return false;

	for( int fx = 0; fx < All_Emulators_Names.count(); fx++ )
	{
		if( All_Emulators_Names[fx] == name &&
			Current_Emulator.Get_Name() != name ) return false;
	}

	return true;
}

void Emulator_Options_Window::Update_Emulator()
{
	if( Update_Info )
	{
		if( ui.RB_Save_Options->isChecked() || // Update emulator bin files info?
			ui.RB_Check_Options->isChecked() )
		{
			QMap<QString, Available_Devices> devList;

			for( int ix = 0; ix < ui.Table_Systems->rowCount(); ix++ )
			{
				if( ! QFile::exists(ui.Table_Systems->item(ix, 1)->text()) )
					continue;

				bool ok = false;
				Available_Devices tmpDev = System_Info::Get_Emulator_Info( ui.Table_Systems->item(ix, 1)->text(), &ok,
																		  Current_Emulator.Get_Version(), ui.Table_Systems->item(ix, 0)->text() );

				if( ok ) devList[ ui.Table_Systems->item(ix, 0)->text() ] = tmpDev;
				else AQGraphic_Warning( tr("Error!"),
										tr("Cannot get emulator info! For file: %1").arg(ui.Table_Systems->item(ix, 1)->text()) );
			}

			Current_Emulator.Set_Devices( devList );
		}
		else if( ui.RB_Check_Version->isChecked() ) // Update version
		{
			VM::Emulator_Version emul_version = VM::Obsolete;

			for( int ix = 0; ix < ui.Table_Systems->rowCount(); ix++ )
			{
				if( QFile::exists(ui.Table_Systems->item(ix, 1)->text()) )
					emul_version = System_Info::Get_Emulator_Version( ui.Table_Systems->item(ix, 1)->text() );
				else
					continue;

				if( emul_version != VM::Obsolete ) break;
			}

			if( emul_version == VM::Obsolete )
				AQGraphic_Warning( tr("Error!"), tr("Cannot get version for emulator!") );
			else
				Current_Emulator.Set_Version( emul_version );
		}
	}
}
