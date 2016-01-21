This module does not do anything by it self.

The idea with the module is to provide a placeholder in the "Manage display" tab on nodes that can hold content that is added programatically to the render array in code in a hook somewhere. The fieldgroups the module creates are divs with configurable classes and it lets you add a comment that describes what is going to be put in the field. The comment is a big help to both the administrator user and the programmers.

So for a use case lets say that I have a node that I would like to put some calculated values in a field for the full-node view mode display. I implement hook_node_view() and put my data in a renderable array and place it in a fieldgroup. This works fine without this module, but what this module simply does is provide a placeholder that programmers can place calculated values in and still let the site administrator drag the fields and grops around in the manage display tab.

So could you not just do this with a node template? Yes, definetly. That is probably the right solution in a lot of cases, but this module lets your administrator user keep the drag and drop flexibility.
