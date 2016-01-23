+ spostare ad_high_per in root, e togliere quel modulo
+ variabile per usare lo script, oppure la chiamata standard
+ xhprof falso di default
- fix nedjo
+ ricreare ad di esempio / fare porting
+ sistemare token, o facendo funzione custom per il token, o mettendo un tracker per le sessioni anonime (con opzione per toglierlo)
+ è possibile fare queueing su database, facendo solo il bootstrap di database?
+ comando per svuotare le queue? (queue-run ...)
+ aggiungere filequeue
+ ripristinare immagini nell'admin/ad usando i placeholder; è possibile fare qualcosa con il size term? Forse si potrebbe usare un view mode? E per avere un filtro?
- in teoria non dovrei aver bisogno di passare il view mode a ad_info, ma potrei prenderlo dalla view
    - in questo modo ogni node type non sarebbe forzato a usare sempre lo stesso view mode
    - e per esempio un modulo che implementa gli ad flash potrebbe avere view mode diversi a seconda della grandezza
    - forse a questo punto ha senso separare ad_example e avere un ad_image.module?
- suite di test, per testare con e senza cache e queue
- istruzioni
  to add and edit ads: http://watchdog.local/admin/ad
  to change the views
- deploy
  drush fr dw dw_blog dw_issue dw_digital_ad
  drush updatedb
  cron ogni minuto con drush queue-run ad
  setting per locale
  creare e configurare /var/www/dev.drupalwatchdod.com/private_files/filequeue/ad/
- cancella dai log tutte le impression senza click; tieni invece iclick con le sue impression collegate
- Se una impression ha generato più click dello stesso tipo, non considerare i click extra
- Sistema a punti che guadagni se superi le query di test
- Togliere token, dovrebbe essere superfluo

****
Turn adv into nodes.
Steps:
+ remove eck definition of the ad entity, and create a new node type in ad_example
+ ad would have an "ad_info" hook which ad_example uses to communicate
  + the ad node type
  + their image field
    - not needed, we'll use view modes
  + their link field
  + which views are used for display
  + which view modes are used for rendering
  - check this info is correct and display a warning if it's not
  + use ad_field_formatter_view in the view mode to automatically display the linked media, and cache the whole rendered entity in that view mode
    + use placeholders for hashes and uniqids; if a link is clicked with still the placeholders, we know it's from an admin
    - extend ad_field_formatter_view to support image styles, so we can have full images in display and thumbs in admin
+ ad would extend those nodes with new properties, like "total clicks" and "total impressions", and would provide links to the statistics
+ move ad administration view to the feature module, because it's custom built for the entity (for example, a site might have some filters specific to its workflow)
+ update ad serve code to use nodes

****
validity checks for clicks:
- clicks without an associated impression
SELECT * FROM eck_tracked_event c
LEFT JOIN eck_tracked_event i ON c.parent_unique_id = i.unique_id
WHERE c.type = 'click' AND i.id IS NULL
- clicks which happened less than 5 seconds after the impression
also check for clicks that happened after too much time
SELECT c.*, c.created - i.created FROM eck_tracked_event c
JOIN eck_tracked_event i ON c.parent_unique_id = i.unique_id
WHERE c.type = 'click' AND c.created - i.created < 5
- clicks whose impression had a different ip
SELECT c.*, c.created - i.created FROM eck_tracked_event c
JOIN eck_tracked_event i ON c.parent_unique_id = i.unique_id
WHERE c.type = 'click' AND c.ip_address != i.ip_address
- clicks whose impression had a different session
SELECT c.*, c.created - i.created FROM eck_tracked_event c
JOIN eck_tracked_event i ON c.parent_unique_id = i.unique_id
WHERE c.type = 'click' AND c.session != i.session
- clicks whose impression had a different user agent
SELECT c.*, c.created - i.created FROM eck_tracked_event c
JOIN eck_tracked_event i ON c.parent_unique_id = i.unique_id
WHERE c.type = 'click' AND c.user_agent != i.user_agent
- clicks whose impression had a different referer
SELECT c.*, c.created - i.created FROM eck_tracked_event c
JOIN eck_tracked_event i ON c.parent_unique_id = i.unique_id
WHERE c.type = 'click' AND c.referer != i.referer
- clicks whose impression had a logged in user
SELECT c.*, c.created - i.created FROM eck_tracked_event c
JOIN eck_tracked_event i ON c.parent_unique_id = i.unique_id
WHERE c.type = 'click' AND c.uid != i.uid AND c.uid > 0
