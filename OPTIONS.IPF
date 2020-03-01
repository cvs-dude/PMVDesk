.*==============================================================*\
.*                                                              *
.* Edit.ipf - Information Tag Language file for the Edit menu   *
.*              help panels.                                    *
.*  Copyright 1991 IBM Corporation                              *
.*                                                              *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Main Edit menu                                              *
.*      res = PANEL_EDIT                                        *
.*--------------------------------------------------------------*/
:h1 res=2310 name=PANEL_EDIT.Options Menu
:i1 id=Edit.Options Menu
:p.
The following commands appear in the Options menu:
:parml tsize=20 break=none.
:hp2.
:pt.Frame controls
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
:pt.Buttons
:ehp2.
:pd.Change the size of your virtual desktop.
:hp2.
:pt.Util + Retrieve
.br
:li.windows
:ehp2.
:pd.Retrieves windows
:eparml.

.*--------------------------------------------------------------*\
.*  Frame Controls command help panel                           *
.*      res = PANEL_EDITUNDO                                    *
.*--------------------------------------------------------------*/
:h1 res=2320 name=PANEL_EDITUNDO.Frame controls
:i2 refid=Edit.Frame Controls
:p.Uncheck "Frame Controls" to hide title and menu bars.
You can also use ALT + F.

.*--------------------------------------------------------------*\
.*  Float on top command help panel                            *
.*      res = PANEL_EDITCUT                                     *
.*--------------------------------------------------------------*/
:h1 res=2330 name=PANEL_EDITCUT.Float on top
:i2 refid=Edit.Float on top
:p.Check "Float on top" to keep PMVDesk the topmost window.
Uncheck to let it behave as a normal window. You can also use ALT + T.

.*--------------------------------------------------------------*\
.*  Retrieve windows on exit command help panel                           *
.*      res = PANEL_EDITCOPY                                    *
.*--------------------------------------------------------------*/
:h1 res=2340 name=PANEL_EDITCOPY.Retrieve windows on exit
:i2 refid=Edit.Retrieve windows on exit
:p.Check "Retrieve windows on exit" to retrieve windows back to
the active desktop when you exit PMVDesk. 
:note.Retrieved windows are hidden. To activate: select
application from Workplace Shell "Window List". Only windows
with child windows (ex. dialog windows) are retrieved visible.

.*--------------------------------------------------------------*\
.*  Util command help panel                          *
.*      res = PANEL_EDITPASTE                                   *
.*--------------------------------------------------------------*/
:h1 res=2350 name=PANEL_EDITPASTE.Util
:i2 refid=Edit.Util
:p."Util" + "Retrieve windows" to retrieve windows back to the
active desktop. 
:note.Retrieved windows are hidden. To activate: select application
from Workplace Shell "Window List". Only windows with child
windows (ex. dialog windows) are retrieved visible.

.*--------------------------------------------------------------*/
:h1 res=2280 name=PANEL_BUTTONS.Buttons
:i2 refid=Edit.Buttons
:p.Change the size of your virtual desktop.
