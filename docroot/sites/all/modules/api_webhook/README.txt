API Webhook
====

This modules provide a web-hook to tell API module to:

- Re-download the code base
- Queue for re-parse on next cron run.

### Install

```
drush dl api_webook
drush en api_webhook -y
```

### Important

This module will remove the old source code to get the new one. Please do not
point a working directory.
