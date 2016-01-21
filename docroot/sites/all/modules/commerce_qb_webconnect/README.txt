Minimal documentation regarding the Drupal Commerce Quickbooks Webconnect Module:

This module is not recommended for non-developers but it can be made to work with some effort.
Switchback, LLC. provides customization, implementation and other development services for
site owners requiring support with this package.

This module requires the Services module and serves as a server plug-in to that module.
You do not need to enable the rest_server or xmlrpc_server components.

Using the Manage Quickbooks Exports admin page will queue existing orders and products.
Customers are automatically queued for export with orders.
Existing payments will not be queued. (Feature request)

Going forward new orders/products/customers/payments will be queued on creation (insert) as part of 
Drupal Commerce API hooks.

Failed exports will be flagged and automatically re-exported upon next sync.
Authentication triggers all exports in ERROR status to be reset to PENDING
Watchdog (Reports -> Recent log messages) will capture errors and should be checked after each sync.

Error 3100 (Item already exists) will be treated as a successful export (Add Customer, Add product, etc.)
This module tracks successful exports to avoid duplication but Quickbooks itself does not prevent
export of duplicate orders (payments too?) so use caution if you have previously used other solutions
to export data from Drupal Commerce as you may end up importing duplicate transactions.

Orders seem to be queued with a higher priority than customers resulting in a first time export
failure for orders associated with new customers. A subsequent export (after customer export) should
be successful. (Bug report?)
