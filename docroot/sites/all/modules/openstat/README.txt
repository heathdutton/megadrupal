ENGLISH VERSION
---------------

INTRODUCTION
------------
The Openstat module integrates Drupal with Openstat statistic service
(https://www.openstat.ru/). This service allows to track visitors, page views,
referring sites, the most popular pages, etc.

FEATURES
--------
- Control of Openstat counter settings via Drupal interface.
- Automatic getting counter ID during installation.
- Control of counter visibility for pages, roles.

REQUIREMENTS
------------
- Registered site at Openstat service.

INSTALLATION
------------
1. Go to https://www.openstat.ru/account/signup page and register your site if
  it have not been registered earlier.
2. Download and unpack the Openstat module directory in your modules folder
   (this is usually "sites/all/modules/"). See https://drupal.org/documentation/
   install/modules-themes/modules-7 for addition information.

CONFIGURATION
-------------
1. Go to Configuration -> System -> Openstat page
   (admin/config/system/openstat). Enter counter ID, select counter type and
   set up addition settings.
2. If you have chosen "Counter with image" go to Structure -> Blocks page
   (admin/structure/block) and place "Openstat counter" block at any region.

MAINTAINER
-----------
* Nickolay Leshchev (Plazik) - https://drupal.org/user/982724

This project has been sponsored by:
* Openstat
  Analytics to tell anything about your website. Visit https://www.openstat.ru/
  for more information.

--------------------------------------------------------------------------------
РУССКАЯ ВЕРСИЯ
--------------

ВВЕДЕНИЕ
--------
Модуль Openstat интегрирует Drupal с сервисом статистики Openstat
(https://www.openstat.ru/). Этот сервис позволяет отслеживать посетителей,
просмотры страниц, ссылающиеся сайты, наиболее популярные страницы и т.д.

ВОЗМОЖНОСТИ
-----------
- Контроль над настройками счетчика Openstat с помощью интерфейса
  Drupal.
- Автоматическое получение ID счетчика во время установки.
- Контроль над отображением счетчика для страниц, ролей.

УСТАНОВКА
---------
1. Перейдите на страницу https://www.openstat.ru/account/signupd и
   зарегистрируйте ваш сайт, если он не был ранее зарегистрирован.
2. Скачайте и распакуйте папку с модулем Openstat в вашу директорию с
   модулями (обычно это "sites/all/modules/"). Смотрите https://drupal.org/docu
   mentation/install/modules-themes/modules-7 для дополнительной информации.

НАСТРОЙКА
---------
1. Перейдите на страницу Конфигурация -> Система -> Openstat
   (admin/config/system/openstat). Введите идентификатор счетчика, выберите тип
   и установите дополнительные настройки.
2. Перейдите на страницу Структура -> Блоки (admin/structure/block) и установите
   блок "Счетчик Openstat" в любой регион.

РАЗРАБОТЧИК
-----------
* Лещев Николай (Plazik) - https://drupal.org/user/982724

Этот проект был спонсирован:
* Openstat
  Аналитика, которая знает все о вашем сайте. Посетите https://www.openstat.ru/
  для подробностей.
