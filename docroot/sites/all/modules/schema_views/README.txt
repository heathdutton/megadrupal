This module adds a tab to Schema module similar to the Inspect tab. Instead of
Schema API code to be pasted in your .install file, though, it generates
scaffolding code to be pasted into your .views.inc file, to give you a headstart
on Views integration for your module. You will still need to complete the code,
such as assigning the correct field handlers and define relationships, but it
takes some of the tediousness out of writing Views integration code.

Note on similar modules
-----------------------
Note, that there is a similarly named module, which also accomplishes the task
of exposing your module's data to Views, Views Schema. I even contemplated 
picking a different name for this module because it would surely cause a lot of
confusion. Schema Views is just what it does, though, so I stuck with the name.

The essential difference between this module, Schema Views, and Views Schema is
that the other module works at runtime; it will analyse your tables and will
generate the Views datastructures on the fly. You may then alter these through
hook_views_data_alter(). This also means that if you use it to get Views
integration for your module's data, your module will need to depend on it, and
thus users of your module will need to install it. This for me was the reason to
actually write Schema Views, because Views Integration did not seem to me
something I should require an extra module for. I did, however, feel the need to
take some of the tediousness out of writing Views integration code.

Schema Views, then, works in a similar fashion to Schema module, which it 
extends. It will generate the required code to write pure, code-only Views
integration, just as you would have written it if you did it all by hand; it
simply gives you a head start over when coding it by hand, with no strings
attached. The users of your module do not need to install any extra modules.
