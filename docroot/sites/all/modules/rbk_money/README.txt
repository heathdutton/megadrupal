The payment module for Ubercart, provides payments through
RBK Money payment system.

Installation and settings
1. Installation. Copy module folder in /sites/all/modules
2. Turn it on at the page /admin/build/modules in “Ubercart
– payment” section
3. Settings. Go to the /admin/store/settings/uc_rbkmoney
3.1. Copy URL from "Payment notification URL" field and
paste it in "Payment notification" field at the RBK Money
merchant settings page
3.2. Enter site ID (from RBK Money merchant settings page)
3.3. Select the currency of payments (RUR by default)
3.4. Enter the secret key. It must be exactly the same as
you specified it at RBK Money merchant settings page

-----------------------------------------------------------
In Russian: платежный модуль для Ubercart, позволяющий
принимать платежи через систему RBK Money. Модуль
отправляет запрос платежа платежной системе, и
обрабатывает возвращаемый ответ, меняя в соответствии с
ним статус заказа.

Установка и настройка
1. Установка. Копируем папку с модулем в каталог
/sites/all/modules
2. Включаем модуль на странице /admin/build/modules,
раздел Ubercart - payment
3. Настройка осуществляется по адресу
/admin/store/settings/uc_rbkmoney.
3.1. Копируем url из поля "URL оповещения о платеже" и
вставляем его в поле "Оповещение о платеже" в личном
кабинете RBK Money
3.2. Прописываем ID сайта (берем из личного кабинета)
3.3. Указываем валюту платежей (по-умолчанию - RUR)
3.4. Указываем в личном кабинете RBK Money cекретный ключ и
прописываем его же в поле "Секретный ключ"
