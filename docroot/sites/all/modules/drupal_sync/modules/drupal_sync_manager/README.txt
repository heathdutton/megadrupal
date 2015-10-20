Description
-----------
Current module is an extension of the basic drupal_sync module.
It presents an administrative section for controlling current content synchronization .

After module enabling in Configuration section of drupal_synс module the
following pages will be added:
1)"Queue of synchronization" admin/config/drupal_sync/drupal_sync_manager-
allows viewing unsent synchronization pack as well as packs unsent by reason of
errors.
2)"Relations of synchronization" admin/config/drupal_sync/drupal_sync_manager/queue/relations
– the list of all connections between websites in synchronized content .
3)"Incoming operations in progress" admin/config/drupal_sync/drupal_sync_manager/local_operations/inprogress
– the list of all synchronization packs that are in progress at the moment. This
page should be blank. It may contain packs that encountered errors during processing.