ENGLISH VERSION
---------------

INTRODUCTION
------------
The LiveInternet module integrates Drupal with Liveinternet statistic service
(http://www.liveinternet.ru/), which most popular in Russia and post-Soviet
states. This service allows to track visitors, page views, referring sites,
the most popular pages, etc. The image with LiveInternet logotype have to be
on every tracking page in free version of counter.

FEATURES
--------
- Full control of LiveInternet counter settings via Drupal interface.
- Support group sites tracking.
- Full control of counter visibility for pages, content types, roles
  (provided by Drupal block system).

REQUIREMENTS
------------
- Registered site at LiveInternet service.

INSTALLATION
------------
1. Go to http://www.liveinternet.ru/add page and register your site if it
   have not been registered earlier. Do NOT choose any counter types
   at http://www.liveinternet.ru/code page. Just skip this page. It will be
   configured in LiveInternet module.
2. Download and unpack the LiveInternet module directory in your modules folder
   (this is usually "sites/all/modules/"). See https://drupal.org/documentation/
   install/modules-themes/modules-7 for addition information.

CONFIGURATION
-------------
1. Go to Configuration -> System -> LiveInternet page
   (admin/config/system/liveinternet). Select counter type, display, color
   scheme and set up addition settings.
2. Go to Structure -> Blocks page (admin/structure/block) and place
   "LiveInternet counter" block at any region.

MAINTAINER
-----------
* Nickolay Leshchev (Plazik) - https://drupal.org/user/982724

--------------------------------------------------------------------------------
РУССКАЯ ВЕРСИЯ
--------------

ВВЕДЕНИЕ
--------
Модуль LiveInternet интегрирует Drupal с сервисом статистики LiveInternet
(http://www.liveinternet.ru/), который особо популярен в России и странах
бывшего СССР. Этот сервис позволяет отслеживать посетителей, просмотры страниц,
ссылающиеся сайты, наиболее популярные страницы и т.д. Изображение с логотипом
LiveInternet должно располагаться на каждой отслеживаемой странице в бесплатной
версии счетчика.

ВОЗМОЖНОСТИ
-----------
- Полный контроль над настройками счетчика LiveInternet с помощью интерфейса
  Drupal.
- Поддержка отслеживания группы сайтов.
- Полный контроль над отображением счетчика для страниц, типов контента, ролей
  (с помощью системы блоков Drupal).

УСТАНОВКА
---------
1. Перейдите на страницу http://www.liveinternet.ru/add и зарегистрируйте ваш
   сайт, если он не был ранее зарегистрирован. НЕ выбирайте никакие настройки
   типов счетчика на странице http://www.liveinternet.ru/code. Просто
   пропустите эту страницу. Все будет настроено в модуле LiveInternet.
2. Скачайте и распакуйте папку с модулем LiveInternet в вашу директорию с
   модулями (обычно это "sites/all/modules/"). Смотрите https://drupal.org/docu
   mentation/install/modules-themes/modules-7 для дополнительной информации.

НАСТРОЙКА
---------
1. Перейдите на страницу Конфигурация -> Система -> LiveInternet
   (admin/config/system/liveinternet). Выберите тип, отображение, цветовую схему
   счетчика и установите дополнительные настройки.
2. Перейдите на страницу Структура -> Блоки (admin/structure/block) и установите
   блок "Счетчик LiveInternet" в любой регион.

РАЗРАБОТЧИК
-----------
* Лещев Николай (Plazik) - https://drupal.org/user/982724
