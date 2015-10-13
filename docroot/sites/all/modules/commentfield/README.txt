Comment Field provides a field that can hold simple comments.
It holds the uid of the author, timestamp, and body (comment text).
The display is configurable, making it possible not to display the author
(for example, in cases where the author is always the same) or timestamp.

This allows administrators to leave simple comments on entities such as
orders, and possibly for users to comment back (if they have the permission to edit the field).

The module doesn't aim to be a full comments solution, which would require
comments as entities, threaded views, anonymous commenting, etc.
