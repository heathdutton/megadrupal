<?php
/**
 * @file
 * Documentation of ces_message.
 */

/**
 * 
 * @defgroup ces_message_doc Ces Message documentation
 * @ingroup ces_message
 * @{
 * 
 * Ces Message
 * ===========
 * 
 * Ces Message gestiona el envío de notificaciones a los usuarios.
 * 
 * Ces Message guardara las notificaciones realizadas a los usuarios para que 
 * puedan verlas y gestionarlas desde la misma web.
 * 
 * A la vez permite decidir si se desea que la notificación le sea enviada por 
 * correo o simplemente guardada en base de datos.
 * 
 * La decisión se puede tomar al enviar la notificación desde el código o 
 * más adelante podría ser una opción del usuario si desea recibir emails y
 * de que tipo. (Pendiente de estudiar).
 * 
 * Plantillas por defecto
 * ----------------------
 * 
 * Las plantillas por defecto se encuentran en includes/actions/.
 * 
 * Cada idioma tiene su propia carpeta y los nombres de los archivos 
 * corresponden a la acción que se realiza.
 * 
 * Ejemplo:
 * 
 * + includes/actions/en/account_activated.inc
 * + includes/actions/ca/account_activated.inc
 * 
 * Implementar una nueva acción
 * ----------------------------
 * 
 * Para implementar una nueva acción se debe incluir en ces_message_install().
 * 
 * Y añadir la correspondiente plantilla por defecto.
 * 
 * El módulo que gestione la acción debe poder gestionar los tokens. 
 * 
 * @see ces_message_install()
 * @see ces_bank_token_info()
 * @see ces_bank_tokens()
 * 
 * Mensajes de usuario
 * -------------------
 * 
 * Los usuarios tienen un listado de todos los mensajes recibidos.
 * 
 * Los mensajes pueden tener diferentes estados:
 * 
 * - 0: Sin leer
 * - 1: Leído
 * - 2: Borrado
 * 
 * Un mensaje borrado no sera mostrado al usuario, no obstante el administrador
 * aún podra verlos, de esta manera tenemos un sistema de registros de 
 * actividad del ces que puede ayudar al administrador de cada exchange.
 * 
 * Lista de pendientes
 * -------------------
 * 
 * @todo Incluir en el listado de los mensajes: 
 * 
 *       - la opción de borrarlos.
 *       - Leido no leido.
 * 
 *       Al mostrar detalle de mensaje marcarlo automáticamente como leído.
 *       Al borrar el mensaje el usuario marcar como borrado, pero mantener en 
 *       la base de datos para que el administrador del exchange pueda tener 
 *       una referencia de la actividad.
 * 
 * @todo Crear ces_message_send_account() para enviar el mensaje a todos los
 *       usuarios de una cuenta.
 * 
 * @todo Estudiar la forma de evitar el exceso de acumulación de mensajes. Por
 *       ejemplo poniendo un limite a los mensajes guardados de cada usuario y
 *       en el caso de sobrepasarlo borrar una cantidad de los más antiguos.
 *       Se debería de estudiar la mejor estrategia.
 * @}
 */
