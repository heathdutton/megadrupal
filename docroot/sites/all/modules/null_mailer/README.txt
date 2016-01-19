Null mailer
-----------

Null mailer makes a backup of your current mailer setting ('mail_system'
system variable), and shows a convenient warning at admin/ and status
report page, to remind you that Null mailer is active.

When disabling Null mailer module, it will restore the previous mailer
setting. If another module overridden 'mail_system' system variable,
while Null mailer module is active, then Null mailer will not restore
it, assuming that you activated another mailer (i.e: SMTP module).

Credits
-------
Developed by Fernando Paredes García of SGC(santexgroup.com) for South
American Explorers(saexplorers.com).
