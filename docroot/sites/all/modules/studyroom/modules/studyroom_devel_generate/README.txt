
-- SUMMARY --

This module will allow you to generate (using devel_generate)
both spaces and reservations.  This will allow you to populate
the system with spaces and reservation to test various elements.

-- REQUIREMENTS -

Studyroom system.
Devel_Generate module.

-- INSTRUCTIONS --

To generate spaces, you will need to first define at least one
location and configure the default hours field.  You can choose
how many spaces to create, choose to delete existing one first,
and whether to use the default hours or pick random opne/close
times. You can also choose maximum random capacity for each space.

To generate reservtions, you will need at least one space.  You
will be able to choose how many to make, how far in advance to
make them, and maximum duration. You can also choose to delete
all existing reservations.
