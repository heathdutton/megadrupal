Messages Watchdog
=================
Logs all site messages to watchdog.

Useful for tracking messages for all users across your site. Even more useful if you have a log query/viewing tool.


Status type mapping
-------------------
'status'  => WATCHDOG_NOTICE  
'warning' => WATCHDOG_WARNING  
'error'   => WATCHDOG_ERROR  

Defaults to WATCHDOG_NOTICE.


Compatibility Note
------------------
This module overrides a theming function, `theme_status_messages`, in order to capture the status messages. As a result, custom theme overrides of status messages is incompatible with this module.
