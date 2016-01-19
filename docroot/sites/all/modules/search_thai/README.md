การตั้งค่า
==========
สำหรับ Drupal 7 เท่านั้น (ณ เวลานี้) คุณมี 2 ทางเลือก โดยใช้ Drush หรือ Drupal UI

Drush commands
------------------
1. `drush en search_thai -y`
2. `drush search-reindex -y`
3. `drush cron`

Drupal UI
-------------
เปิดใช้งานเหมือน module ทั่วไป แล้วก็ ...

1. ไปที่หน้าตั้งค่าการค้นหา `/admin/config/search/settings`
2. ถ้าต้องการเปลี่ยน __จำนวนตัวอักษรที่จะอินเด็กซ์__ ก็ระบุที่นั่น ค่า default คือ `3`
3. ถ้า __Simple Thai handling__ ยังไม่ได้ติ๊กไว้ ก็ติ๊กซะตรงนั้น
4. Re-index เว็บไซต์ของคุณ
5. รัน cron จนกว่าจะครบ 100%

[ขัดข้องสงสัย?](http://webzer.net/contact)


CONFIGURATION
=============
For Drupal 7 only (at the moment). You have 2 options via Drush or Drupal UI.

Drush commands
------------------
1. `drush en search_thai -y`
2. `drush search-reindex -y`
3. `drush cron`

Drupal UI
-------------
Enable the module as usual, then ...

1. Go to search settings page `/admin/config/search/settings`
2. If you want to change default __Minimum word length to index__, input it there. Default is `3`.
3. If __Simple Thai handling__ is not checked by default, tick it there.
4. Re-index your site.
5. Run cron till you get 100% indexed.

[Any question?](http://webzer.net/contact)
