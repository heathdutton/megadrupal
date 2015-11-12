Block Machine Name is a small module that adds a machine name field to custom blocks.
It's primary intent is to solve the problem of styling and rendering blocks that use 
numeric auto-increment ID's which may later not match their production values when 
the blocks are reproduced.
Machine names can only contain lowercase letters, numbers, and underscores.

Classes:
If you give your custom block a machine name such as 'my_cool_block', the block will 
be given a class of 'block-my-cool-block' (underscores will be converted to dashes).

Template Suggestions:
The module will also provide a template suggestion - in the above example, you could 
create a tpl named 'block--my-cool-block.tpl.php' (tpl files always use dashes).