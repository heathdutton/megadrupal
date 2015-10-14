LOGIC BLOCKS
=================

Speed form blocks is a single module (enabled via the modules page) which :

Allows a block to be created which can process another block's content. So
if you wish to stop a block displaying certain words that is possible, or if you
want the block to not display if it is very long, that is possible as well.

The full list is 

1) ACTION
-- Concatenate two blocks together
-- Strip class=content from around a block
-- Add a prefix to a block
-- Replace content within a block
-- Add a suffix to a block
-- Swap a block for another
-- Wrap a block with HTML

2) LOGIC
-- Show a different block if the block contains certain content
-- Show a different block if the block is empty
-- Show a different block if the block's content is over, or under a certain amount

3) NODES
-- Show a different block depending on the node's language
-- Show a different block if the node is published
-- Show a different block depending on the node type

4) USER
-- Show a different block depending on user ID
-- Show a different block depending on when a user was last active
-- Show a different block depending on a role
-- Show a different block depending on language
-- Show a different block depending on timezone

Developers can add new logic and options, or tweak existing options by duplicating
current files and renaming them (and the functions contained within).

The module doesn't have any hooks. Please let me know if you'd find one 
useful. Always happy to take on board ideas for improvements (when time 
permits).

The module is not known to cause any conflicts and doesn't require any other 
modules.

Please contact me via my drupal page or www.pgogy.com.


