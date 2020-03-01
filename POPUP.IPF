.*==============================================================*\
.*                                                              *
.* File.ipf - Information Tag Language file for the File menu   *
.*              help panels.                                    *
.*                                                              *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Main file menu                                              *
.*      res = PANEL_POPUP                                       *
.*--------------------------------------------------------------*/
:h1 res=2200 name=PANEL_POPUP.Popup Menu
:i1 id=Popup.Popup Menu
:p.To activate press mouse button 2, or press SHIFT + F10.
:p.The following commands appear in the popup menu:
:parml tsize=20 break=none.
:hp2.
:pt.Help
:ehp2.
:pd.Default help menu
:hp2.
:pt.Window
:ehp2.
:pd.System menu
:hp2.
:pt.Util + Retrieve
.br
:li.windows
:ehp2.
:pd.Retrieves windows
:hp2.
:pt.Buttons
:ehp2.
:pd.Change the size of your virtual desktop.
:hp2.
:pt.Frame Controls
:ehp2.
:pd.Hide/Unhide title and menu bars
:hp2.
:pt.Float on top
:ehp2.
:pd.Keep PMVDesk the topmost window
:hp2.
:pt.Retrieve windows
.br
:li.on exit
:ehp2.
:pd.Retrieves windows on exit
:hp2.
:pt.Cancel
:ehp2.
:pd.Cancel any selection, and quit popup menu
:eparml.

.*--------------------------------------------------------------*/
:h1 res=2210 name=PANEL_POPUPHELP.Help
:i2 refid=Popup.Help
:p. By pressing "Help", the "Help index" will open.
:p. By pressing the tiny button with a tiny arrow, you open a
submenu. From this submenu you can choose between:
:ul.
:li.Help index.
:eul.
:ul.
:li.General help.
:eul.
:ul.
:li.Using help.
:eul.
:ul.
:li.Keys help.
:eul.
:ul.
:li.Product information.
:eul.

.*--------------------------------------------------------------*/
:h1 res=2220 name=PANEL_WINDOW.Window
:i2 refid=Popup.Window
:p. By pressing "Window", you open a system submenu. From this submenu
you can choose between:
:ul.
:li.Move. Move window.
:eul.
:ul.
:li.Size. Size window.
:eul.
:ul.
:li.Hide. Hide window.
:eul.
:ul.
:li.Close. Close window and quit application.
:eul.


.*--------------------------------------------------------------*/
:h1 res=2230 name=PANEL_UTIL.Util
:i2 refid=Popup.Util
:p. By pressing "Util", you open a submenu. From this submenu
you can choose:
:ul.
:li.Retrieve windows. Retrieve windows back to the
active desktop. 
:note.Retrieved windows are hidden. To activate: select application
from Workplace Shell "Window List". Only windows with child
windows (ex. dialog windows) are retrieved visible.
:eul.


.*--------------------------------------------------------------*/
:h1 res=2240 name=PANEL_FRAMECONTROLS.Frame controls
:i2 refid=Popup.Frame controls
:p.Check "Frame Controls" to show title and menu bars.
You can also use ALT + F.


.*--------------------------------------------------------------*/
:h1 res=2250 name=PANEL_FLOATONTOP.Float on top
:i2 refid=Popup.Float on top
:p.Check "Float on top" to keep PMVDesk the topmost window.
Uncheck to let it behave as a normal window. You can also use ALT + T.


.*--------------------------------------------------------------*/
:h1 res=2260 name=PANEL_RETRIEVE.Retrieve windows on exit
:i2 refid=Popup.Retrieve windows on exit
:p.Check "Retrieve windows on exit" to retrieve windows back to
the active desktop when you exit PMVDesk. 
:note.Retrieved windows are hidden. To activate: select
application from Workplace Shell "Window List". Only windows
with child windows (ex. dialog windows) are retrieved visible.


.*--------------------------------------------------------------*/
:h1 res=2270 name=PANEL_CANCEL.Cancel
:i2 refid=Popup.Cancel
:p.Cancel any selection, and quit popup menu.

.*--------------------------------------------------------------*/
:h1 res=2280 name=PANEL_BUTTONS.Buttons
:i2 refid=Popup.Buttons
:p.Change the size of your virtual desktop.

