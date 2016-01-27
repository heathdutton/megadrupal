Nodequeue Suggestions 

This module allows the administrator to specify that a certain nodequeue
contains suggestions for another nodequeue.

To use this functionality, create two node queues, and specify that the second
queue has the first queue attached as a "suggestion queue", by checking the
box and choosing the second queue.

Nodes placed in the queue specified for suggestions, will now be treated as
suggestions for the main node queue.

Go to admin/structure/nodequeue/suggestions and see at list of all subqueues of
the main and suggestion queues, and the number of suggestions in each, and
click moderate to move the nodes into the main queues.


Developer notes:

This module is based on code taken from the nodequeue and block modules, and
modified for this purpose. The dragging is based on the drupal tabledrag
features but assisted by javascript from the block module.

The mapping between the subqueues in the main and suggestion queue is based on
the reference field in the subqueues.

The mapping between main and suggestion queues are saved in the variable named
"nodequeue_suggestions_queues".

Currently, this module requires the latest 7.x-2.x with this patch to work properly.
http://drupal.org/node/1978764#comment-7339098
