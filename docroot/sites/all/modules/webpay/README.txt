
/*****************************************************************************
****  English version        *************************************************
******************************************************************************/
Webpay Module

This module helps with the integration and makes possible to build the specific
module for the specific type of commerce (commerce, ubercart, ...other)
IT DOES NOT WORK BY ITSELF.

Requirements


"Kit de Conexión de Comercio" (KCC) version 6.0
To perform Webpay integration the commerce site must have installed KCC.
It can be downloaded from:
https://www.transbank.cl/public/16_productos_y_servicios.html


Apache and PHP configuration

It needs to have some PHP and Apache features set as disabled. 
The following must be disabled from your server:
mod_security  Apache
safe_mode  PHP

The server needs to be able to execute CGI
Apache needs permission to logs (in KCC folder) and tbk_config.dat

Transbank's documentation defined the permission for each file on KCC. If Apache
can not write the file datos/tbk_config.dat , you must edit the file manually.
 

Considerations


This module just perform conection to Webpay in dev or prod enviroments. On this
last case the commerce site must get a key from Transbank and after a (long)
period of certification the site will be ready.


/*****************************************************************************
****  Versión en Castellano  *************************************************
******************************************************************************/
Módulo Webpay


Este módulo facilita la integración del método de pago Webpay de Transbank con 
Drupal. Este método es el más usado en Chile, el cual permite pagar con tarjetas
 de crédito o con tarjeta de débito (Redcompra) en caso de utilizar el sistema 
 en modo nacional (pagar con pesos chilenos).
Es necesario instalar módulos específicos para tener la funcionalidad completa,
que sean específicos para el comercio que se utiliza (commerce, ubercart u otro)
ESTE MÓDULO NO HACE LA INTEGRACIÓN POR SÍ MISMO.

 
Características


- Cuenta con un panel de administración para configurar los parámetros del
 KCC y otros con respecto al funcionamiento con Drupal.

- Se registran todas las transacciones realizadas entre el comercio y Webpay.

- Cuenta con un módulo para realizar rápidamente una prueba de conexión con 
Webpay.


Requisitos


Kit de Conexión de Comercio (KCC) versión 6.0

Para integrarse con Webpay, el comercio debe tener instalado el sistema KCC de 
Transbank, el cual puede ser descargado desde:
https://www.transbank.cl/public/16_productos_y_servicios.html

Se debe instalar en la carpeta cgi-bin, ubicado en el núcleo de instalación de 
Drupal.


Configuraciones Apache y PHP


Para que el sistema de KCC pueda operar en el servidor del comercio, se necesita
 tener en cuenta que algunos parámetros de Apache y de PHP deben estar 
 deshabilitados, en caso contrario puede no haber conexión con Webpay. 
 Los parámetros son:
mod_security de Apache
safe_mode de PHP
Adicionalmente, el servidor debe ejecutar programas CGI.
Permisos de escritura

La documentación de Transbank define los permisos que deben tener cada archivo 
del sistema de KCC, adicionalmente, Apache debe contar con permisos de escritura
 sobre el archivo datos/tbk_config.dat, esto para configurar sus parámetros 
 desde la administración de Drupal, en caso contrario, se deberá hacer los 
 cambios sobre el archivo directamente.
 
 

Consideraciones


Este módulo sólo permite la conexión con Webpay, sea en Certificación o en 
Producción. Para este último, es responsabilidad del comercio conseguir el 
código por parte de Transbank, el cual requerirá de un periodo para certificar 
el sistema por completo (tener acceso a esta y realizar pruebas de compras) 
para contar con la aprobación y operar en producción.
