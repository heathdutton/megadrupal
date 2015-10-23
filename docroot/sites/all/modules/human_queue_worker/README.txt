Human Queue Worker
==================

This module creates a UI for human users to work on entities which have been
placed in the system queue. This is intended for entities which require some
sort of processing which an automated system cannot do, such as approving new
content or cleaning up data.

Multiple users may concurrently work on processing items from system queues.
Items are claimed and deleted in the same way as with automated workers.

Queues that are to be worked on by humans are declared by implementing
hook_human_queue_worker_info(), in a manner similar to implementing
hook_cron_queue_info(). This defines one or more operations that users may
perform on each entity, using Entity Operations module.

Items to be worked on are queued in the normal manner. Any queue backend may be
used.

Users should be granted permissions for the queues, as well as the permissions
relating to the operations they are to perform on the entities.

A number of operations are provided by this module and also Entity Operations
which may be useful:
 - EntityOperationsOperationDeleteConfirm: Deletes the entity, and shows a
   confirmation checkbox in the form to prevent accidental clicking of the
   button.
 - HumanQueueWorkerOperationSkip: Skips the entity for now, by re-queueing it at
   the end of the queue.
 - HumanQueueWorkerOperationMoveQueue: Moves the entity to a different queue,
   where, for example, more detailed processing can be done on it, or for the
   attention of an administrator.
 - EntityOperationsOperationPublish/EntityOperationsOperationUnPublish:
   publish or unpublish the entity, respectively.
