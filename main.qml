/**********************************************************************************
    ForecastingSAT version 1.0. Calculation of the position and angles
    of near space satellites
    Copyright (C) 2020  Maxim Shershavikov

    This file is part of ForecastingSAT version 1.0.

    ForecastingSAT v1.0 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ForecastingSAT v1.0 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Email m.shershavikov@yandex.ru
    To read a copy of the GNU General Public License in a file COPYING.txt,
    to do this, click the AbautProgram button.
**********************************************************************************/

import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import hub 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 1000
    minimumWidth: 1000
    maximumWidth: 1000
    height: 307
    minimumHeight: 335
    maximumHeight: 335
    title: qsTr("ForecastingSAT eng v1.0")

    Hub {
        id: hub
        onTotextfield_tle: {
            textField_opentle.text = open_tle
        }
        onTotextfield_open: {
            textField_opentxt.text = open_txt
        }
        onTotextfield_save: {
            textField_savetxt.text = save_txt
        }
        onTomessagedialog_mes: {
            window_message.visible = true
            text_message.visible = true
            text_message.text = message
            blocked_main_window.visible = true
        }
        onTomessagedialog_error: {
            window_error.visible = true
            text_error.visible = true
            text_error.text = error
            blocked_main_window.visible = true
        }
        onToprogressbar_visible: {
            text_proc.visible = visible
            text_progress_value.visible = visible
        }
        onCount: {
            progressBar.value = count_next
            text_progress_value.text = count_next
        }
        onToflag: {
            button_start.flag = flag
        }
    }

    Button {
        id: button_addtle
        x: 607
        y: 27
        width: 144
        height: 48
        text: qsTr("Add TLE")
        onClicked: fileDialog_opentle.open()
    }

    Button {
        id: button_addtxt
        x: 607
        y: 135
        width: 144
        height: 48
        text: qsTr("Add txt")
        onClicked: fileDialog_opentxt.open()
    }

    Button {
        id: button_savetxt
        x: 607
        y: 81
        width: 144
        height: 48
        text: qsTr("Save txt")
        onClicked: fileDialog_savetxt.open()
    }

    Button {
        id: button_abautprogramm
        x: 778
        y: 135
        width: 194
        height: 48
        text: qsTr("About the program")
        onClicked: {
            blocked_main_window.visible = true
            window_abaut_program.visible = true
            text_abaut_programm.visible = true
        }
    }

    Button {
        id: button_start
        x: 778
        y: 27
        width: 194
        height: 102
        text: "Calculate"
        rightPadding: 8
        font.kerning: true
        font.preferShaping: true
        flat: false
        highlighted: false
        autoRepeat: false
        autoExclusive: false
        checked: false
        checkable: false
        property var flag: false
        onClicked:
        {
            if (flag == false)
            {
                flag = true
                hub.startmain(switch_mode.checked, switch_from.checked, textfield_lat.text, textfield_lon.text,
                              textfield_date_start.text, textfield_date_end.text, textfield_from_lat.text, textfield_from_lon.text)
            }
            else
            {                
                window_message.visible = true
                text_message.visible = true
                text_message.text = "The calculation is in progress. Wait for the end"
                blocked_main_window.visible = true
            }
        }
    }

    FileDialog {
        id: fileDialog_opentle
        title: "Add tle"
        nameFilters: "*.tle"
        onAccepted: {
            hub.pathfile_tle(this.fileUrl);
        }
    }

    FileDialog {
        id: fileDialog_opentxt
        title: "Add txt"
        nameFilters: "*.txt"
        onAccepted: {
            hub.pathfile_open(this.fileUrl);
        }
    }

    FileDialog {
        id: fileDialog_savetxt
        title: "Save txt"
        selectExisting: false
        onAccepted: {
            hub.pathfile_save(this.fileUrl);
        }
    }

    TextField {
        id: textField_opentle
        x: 36
        y: 36
        width: 565
        height: 39
        font.underline: false
        font.preferShaping: false
        font.kerning: false
        readOnly: true
        selectByMouse: true
    }

    TextField {
        id: textField_opentxt
        x: 36
        y: 144
        width: 565
        height: 39
        font.underline: false
        font.preferShaping: false
        font.kerning: false
        readOnly: true
        selectByMouse: true
    }

    TextField {
        id: textField_savetxt
        x: 36
        y: 90
        width: 565
        height: 39
        font.underline: false
        font.preferShaping: false
        font.kerning: false
        readOnly: true
        selectByMouse: true
    }

    Switch {
        id: switch_mode
        x: 36
        y: 222
        text: qsTr("Enable calculation by latitude and longitude")
        onClicked: {
            switch (switch_mode.position)
            {
            case 0:
                button_addtxt.visible = true
                textField_opentxt.visible = true
                text_lat.visible = false
                text_lon.visible = false
                textfield_lat.visible = false
                textfield_lon.visible = false
                text_date_start.visible = false
                textfield_date_end.visible = false
                text_date_end.visible = false
                textfield_date_start.visible = false
                switch_from.visible = false
                text_from_lat.visible = false
                textfield_from_lat.visible = false
                text_from_lon.visible = false
                textfield_from_lon.visible = false
            break;
            case 1:
                button_addtxt.visible = false
                textField_opentxt.visible = false
                text_lat.visible = true
                text_lon.visible = true
                textfield_lat.visible = true
                textfield_lon.visible = true
                text_date_start.visible = true
                textfield_date_end.visible = true
                text_date_end.visible = true
                textfield_date_start.visible = true
                switch_from.visible = true
                if (switch_from.position == 1)
                {
                    text_from_lat.visible = true
                    textfield_from_lat.visible = true
                    text_from_lon.visible = true
                    textfield_from_lon.visible = true
                }
            break;
            }
        }
    }

    Text {
        id: text_lon
        x: 177
        y: 135
        width: 120
        height: 24
        color: "#ffffff"
        text: qsTr("Longitude")
        font.pixelSize: 18
        horizontalAlignment: Text.AlignHCenter
        visible: false
    }

    Text {
        id: text_lat
        x: 36
        y: 135
        width: 120
        height: 24
        color: "#ffffff"
        text: qsTr("Latitude")
        font.pixelSize: 18
        horizontalAlignment: Text.AlignHCenter
        visible: false
    }

    TextField {
        id: textfield_lat
        x: 36
        y: 152
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 13
        visible: false
        validator: RegExpValidator {regExp: /^[-0-9.]+$/}
    }

    TextField {
        id: textfield_lon
        x: 177
        y: 152
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 13
        visible: false
        validator: RegExpValidator {regExp: /^[-0-9.]+$/}
    }

    Text {
        id: text_date_start
        x: 325
        y: 135
        width: 120
        height: 24
        color: "#ffffff"
        text: qsTr("Start date")
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 18
        visible: false
    }

    TextField {
        id: textfield_date_start
        x: 325
        y: 152
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 13
        visible: false
        validator: RegExpValidator {regExp: /^[0-9.]+$/}
    }

    Text {
        id: text_date_end
        x: 471
        y: 135
        width: 120
        height: 24
        color: "#ffffff"
        text: qsTr("Final date")
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 18
        visible: false
    }

    TextField {
        id: textfield_date_end
        x: 471
        y: 152
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 13
        visible: false
        validator: RegExpValidator {regExp: /^[0-9.]+$/}
    }

    Switch {
        id: switch_from
        x: 380
        y: 222
        text: qsTr("Enterred Offsets")
        visible: false
        onClicked: {
            switch (switch_from.position)
            {
            case 0:
                text_from_lat.visible = false
                textfield_from_lat.visible = false
                text_from_lon.visible = false
                textfield_from_lon.visible = false
            break;
            case 1:
                text_from_lat.visible = true
                textfield_from_lat.visible = true
                text_from_lon.visible = true
                textfield_from_lon.visible = true
            break;
            }
        }
    }

    Text {
        id: text_from_lat
        x: 619
        y: 201
        width: 120
        height: 24
        color: "#ffffff"
        text: qsTr("+/- from latitude")
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 18
        visible: false
    }

    TextField {
        id: textfield_from_lat
        x: 619
        y: 225
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 13
        visible: false
        validator: RegExpValidator {regExp: /^[0-9.]+$/}
    }

    Text {
        id: text_from_lon
        x: 815
        y: 201
        width: 120
        height: 24
        color: "#ffffff"
        text: qsTr("+/- from longitude")
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 18
        visible: false
    }

    TextField {
        id: textfield_from_lon
        x: 815
        y: 225
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 13
        visible: false
        validator: RegExpValidator {regExp: /^[0-9.]+$/}
    }

    ProgressBar {
        id: progressBar
        x: 36
        y: 306
        width: 936
        height: 11
        from: 0.0
        to: 100.0
        value: 0
    }

    Text {
        id: text_proc
        x: 945
        y: 283
        width: 27
        height: 24
        color: "#ffffff"
        text: qsTr("%")
        horizontalAlignment: Text.AlignHCenter
        visible: false
        font.pixelSize: 18
    }

    Text {
        id: text_progress_value
        x: 915
        y: 283
        width: 37
        height: 24
        color: "#ffffff"
        text: qsTr("0")
        horizontalAlignment: Text.AlignHCenter
        visible: false
        font.pixelSize: 18
    }

    Rectangle {
        id: blocked_main_window
        visible: false
        color: Qt.rgba(0.0, 0.0, 0.0, 0.5)
        anchors.fill: parent
        MouseArea {
            anchors.fill: parent
        }
    }

    ApplicationWindow {
        id: window_error
        visible: false
        width: 800
        minimumWidth: 800
        maximumWidth: 800
        height: 250
        minimumHeight: 250
        maximumHeight: 250
        title: qsTr("Error")
        flags: Qt.Window | Qt.WindowTitleHint | Qt.CustomizeWindowHint

        Text {
            id: text_error
            x: 20
            y: 20
            width: 750
            height: 150
            color: "#ffffff"
            wrapMode: TextEdit.Wrap
            visible: false
            font.pixelSize: 18
         }

         Button {
            id: button_error_ok
            x: 650
            y: 180
            width: 120
            height: 48
            text: qsTr("OK")
            onClicked: {
                text_error.visible = false
                blocked_main_window.visible = false
                progressBar.value = 0
                text_proc.visible = false
                text_progress_value.visible = false
                window_error.close()
            }
        }
    }

    ApplicationWindow {
        id: window_message
        visible: false
        width: 800
        minimumWidth: 800
        maximumWidth: 800
        height: 250
        minimumHeight: 250
        maximumHeight: 250
        title: qsTr("Message")
        flags: Qt.Window | Qt.WindowTitleHint | Qt.CustomizeWindowHint

        Text {
            id: text_message
            x: 20
            y: 20
            width: 750
            height: 150
            color: "#ffffff"
            wrapMode: TextEdit.Wrap
            visible: false
            font.pixelSize: 18
         }

         Button {
            id: button_message_ok
            x: 650
            y: 180
            width: 120
            height: 48
            text: qsTr("OK")
            onClicked: {
                text_message.visible = false
                blocked_main_window.visible = false
                window_message.close()
                if (progressBar.value == 100)
                {
                    progressBar.value = 0
                    text_proc.visible = false
                    text_progress_value.visible = false
                }
            }
        }
    }

    ApplicationWindow {
        id: window_abaut_program
        visible: false
        width: 800
        minimumWidth: 800
        maximumWidth: 800
        height: 700
        minimumHeight: 700
        maximumHeight: 700
        title: qsTr("About the program")
        flags: Qt.Window | Qt.WindowTitleHint | Qt.CustomizeWindowHint

        Text {
            id: text_abaut_programm
            x: 20
            y: 20
            width: 750
            height: 150
            color: "#ffffff"
            wrapMode: TextEdit.Wrap
            visible: false
            font.pixelSize: 18 
            text: "Calculated by date, the input data format is a text file with dates and times. Date example: 20.08.2020 08.05.18 The date and time must be entered strictly as in the example. \
When calculating latitude and longitude, you need to enter latitude and longitude and a time period. Date input example: 01.02.2020. The date must be entered strictly as in the example. \
If you need to get more angles, you need to set the offset from latitude and longitude.\n
ForecastingSAT version 1.0. Calculation of the position and angles
of near space satellites

Copyright (C) 2020  Maxim Shershavikov

ForecastingSAT v1.0 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ForecastingSAT v1.0 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

Email m.shershavikov@yandex.ru
To read a copy of the GNU General Public License in a file COPYING.txt,
to do this, click the AbautProgram button."
         }

         Button {
            id: button_abaut_programm_ok
            x: 650
            y: 630
            width: 120
            height: 48
            text: qsTr("OK")
            onClicked: {
                blocked_main_window.visible = false
                window_abaut_program.close()
            }
        }
    }
}
