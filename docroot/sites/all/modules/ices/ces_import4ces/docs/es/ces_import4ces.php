<?php
/**
 * @file
 * Documentation of ces_import4ces.
 */

/**
 * 
 * @defgroup ces_import4ces_doc Ces Import from CES documentation
 * @ingroup ces_import4ces
 * @{
 * 
 * Ces Import from CES
 * ===================
 * 
 * La instalación de ces_import4ces crea las tablas necesarias en la base de 
 * datos para tener una relación de las importaciones realizadas.
 * 
 * Los campos que no sean recogidos en la importación seran guardados en el 
 * registro correspondiente en el campo data serializados.
 * 
 * Las posibles incidencias en la importación que deban ser recogidas para 
 * tener constancia de ellas se guardaran en el campo "observations" de la 
 * tabla ces_import4ces_exchange.
 * 
 * tablas:
 * 
 * @code
 * ces_import4ces_exchange (
 *       id,
 *       exchance_id,
 *       date,
 *       step,          ( Paso en el que nos hemos quedado )
 *       row,           ( última fila importada )
 *       uid,           ( Identificador de usuario que realiza la importación )
 *       observations   ( Incidencias en la importación )
 *       data           ( campos que no recogemos serialzados )
 *       )
 * 
 * ces_import4ces_objects (
 *       id, 
 *       import_id,
 *       object,      ( Tipo de dato: usuario, transacción, etc.. )
 *       object_id,   ( Identificador de objeto )
 *       data         ( campos que no recogemos serialzados )
 *       )
 * @endcode
 * 
 * El usuario administrativo del nuevo banco sera el que realiza la
 * importación.
 * 
 * Al activar una exchage se crea la cuenta 0000, relacionada con el usuario
 * de drupal $exchange['admin']. Cuando se crea una exchange mediante 
 * formulario web, efectivamente se crea este usuario drupal, pero se hace 
 * en el código del módulo y no en el código de la librería, puesto que es 
 * algo específico de drupal.
 * 
 * Este usuario drupal es el que, en la activación del exchange, recibe los
 * permisos de administración de la exchange, por lo que si no hay ningún 
 * usuario real el exchange no se podría administrar (salvo quizás por el
 * usuario 1).
 * 
 * Paralelamente, al activar el exchange se crea una primera cuenta para este
 * usuario, para que también pueda hacer tareas administrativas que impliquen
 * trasacciones a su cuenta (a image y semejanza del ces), pero podríamos 
 * decir que la cuestión de la cuenta no es tan relevante como el hecho de 
 * que exista un usuario con los permisos de administracion del exchange.
 * 
 * A efectos de migración, este usuario es el 0000 que tienen todas las redes
 * y que es especial para administración.
 * 
 * Estratègia
 * 
 *   Usuaris
 * 
 * Lista de tareas pendientes por orden de prioridad
 * -------------------------------------------------
 * 
 * @todo Subarea Aquest és un concepte que no està implementat en el CES.
 *       Decidir què fem.
 * 
 * @}
 */
