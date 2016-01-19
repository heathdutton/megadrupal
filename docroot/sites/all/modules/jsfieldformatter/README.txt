This project allows formatting a text field as a javascript behaviour.

Security wise, this means it will output unsanitized data to the screen without
any permission check.

Only use this formatter with fields entered by trusted users. If this module is
installed, anyone who can configure formatters for text fields cold configure
this formatter, so only install this module if users configuring field formatters
can input javascript.
