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

#ifndef ADVANCED_SETTINGS_WINDOW_H
#define ADVANCED_SETTINGS_WINDOW_H

#include <QSettings>
#include "VM_Devices.h"
#include "ui_Advanced_Settings_Window.h"

class Settings_Widget;

class Advanced_Settings_Window: public QDialog
{
	Q_OBJECT

	public:
		Advanced_Settings_Window( QWidget *parent = 0 );
        ~Advanced_Settings_Window();

	public slots:
		void done(int);

	private slots:
        /// Old Setttings Window
		void on_Button_Create_Template_from_VM_clicked();
		void on_TB_VM_Folder_clicked();
		void CB_Language_currentIndexChanged( int index );
		void CB_Icons_Theme_currentIndexChanged( int index );
		void VNC_Warning( bool state );
		void Load_Templates();
        ///

		void on_TB_Browse_Before_clicked();
		void on_TB_Browse_After_clicked();
		void on_TB_Log_File_clicked();
		void on_TB_Screenshot_Folder_clicked();
		void on_TB_QEMU_IMG_Browse_clicked();

		void on_TB_Add_Emulator_clicked();
		void on_TB_Delete_Emulator_clicked();
		void on_TB_Edit_Emulator_clicked();
		void on_TB_Use_Default_clicked();
		void on_TB_Find_All_Emulators_clicked();
		void on_Emulators_Table_cellDoubleClicked( int row, int column );
		void on_Button_CDROM_Add_clicked();
		void on_Button_CDROM_Edit_clicked();
		void on_Button_CDROM_Delete_clicked();

		bool Load_Emulators_Info();
		bool Save_Emulators_Info();
		void Update_Emulators_Info();

		QStringList Get_All_Emulators_Names() const;

	private:
		Ui::Advanced_Settings_Window ui;
		QSettings Settings;
        Settings_Widget* settings_widget;

		QList<Emulator> Emulators;
};

#endif
