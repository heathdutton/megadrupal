### 01.10.2015
* Добавлены новые платежные методы QW,QP

### 02.08.2015
* Fix bugs with success url

### 01.07.2015
* Added statistics

### 29.04.2015
* Added support multilanguage (English, Russian)
* Update icons

### 23.04.2015
* Fixed repeated paymentAviso calls incorrect answer, when the order status was
STATUS_PAYED

### 04.07.2014
* Fixed payments by anonymous users
* Fixed transaction.inc loading bugs
* Cleanup variables on uninstall
* Fixed "No transactions" message markup
* Update Ubercart integration module

### 25.06.2014
* Refactor `yamoney` and `ya_commerce` modules
* Fixed `ya_commerce` to work with Commerce Payment module instead of Commerce Order (now fully integrated with standard checkout process)
* Fixed user transactions page access checks
* Fixed Yandex.Money server ip access checks
* Fixed variables names to avoid collisions to other modules
* New internal transaction API
* Added Yandex.Money API configuration page to admin/config/system
* Added transactions log to admin/reports
* Added `hook_complete_page()` to perform tasks when payment completed
* Added `hook_fail_page()` to perform tasks when payment failed
* Added `hook_complete_page_alter()` to alter success page
* Added `hook_fail_page_alter()` to alter fail page
* `yamoney/temp` callback now passes arguments to target pages
* Added proper error handling in `checkOrder` and `paymentAviso` callbacks
* Added `hook_yamoney_check_order_alter()` to perform tasks on `checkOrder` requests
* Added `hook_yamoney_process_payment_alter()` to perform tasks on `paymentAviso` requests
* Added option to select default payment method
* Added `hook_yamoney_order_submission_params_alter()`, `hook_yamoney_shop_params_alter()` and `hook_yamoney_quick_params_alter()` to define custom arguments for order submission request
* Removed Drupal Commerce and Ubercart code from core API module
