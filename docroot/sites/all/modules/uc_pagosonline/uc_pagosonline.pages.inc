<?php
// $Id: uc_pagosonline.pages.inc,v 1.1 2010/10/14 04:03:29 juankvillegas Exp $

/**
 * @file
 * PagosOnline module scripts to check the transaction.
 */

function _uc_pagosonline_confirmation() {
    global $user;
    $pol = array();
    $pol['ip'] = '';
    foreach ($_REQUEST as $key => $value) {
        $pol[$key] = check_plain($_REQUEST[$key]);
    }
    $pol['order_id'] = _uc_pagosonline_get_order_id($pol['ref_venta']);
    $pol['cart_id'] = $pol['extra2'];
    $order = uc_order_load($pol['order_id']);
    if (drupal_strtoupper($pol['firma']) != drupal_strtoupper(uc_pagosonline_firma($pol['usuario_id'], $pol['ref_venta'], $pol['valor'], $pol['moneda'], $pol['estado_pol']))) {
        $variables  = array(
            '!order_id' => $pol['order_id'],
            '!ip' => $pol['ip'],
            );
        $severity   = WATCHDOG_ALERT;
        $link       = NULL;
        if (_uc_pagosonline_is_confirmation()) {
            uc_order_comment_save($pol['order_id'], $user->uid ? $user->uid : 0, t("Trying to pay order !order_id, but the digital signature doesn't match. IP !ip", $variables));
            watchdog('PagosOnline', "Trying to pay order !order_id, but the digital signature doesn't match. IP !ip", $variables, $severity, $link);
        }
        else {
            drupal_set_message(t("We can't confirm the authenticity of this transaction.", $variables), 'error');
        }
    }
    else {
        if ($order === FALSE) {
            $variables  = array(
                '!order_id' => $pol['order_id'],
                '!ip' => $pol['ip'],
                );
            $severity   = WATCHDOG_ALERT;
            $link       = NULL;
            if (_uc_pagosonline_is_confirmation()) {
                watchdog('PagosOnline', "Trying to pay order !order_id but it doesn't exists. IP !ip", $variables, $severity, $link);
            }
            else {
                drupal_set_message(t("Trying to pay order !order_id but it doesn't exists.", $variables), 'error');
            }
        }
        if ($pol['ref_venta'] != uc_pagosonline_get_reference($pol['order_id'])) {
            $variables  = array(
                '!order_id' => $pol['order_id'],
                '!ref_val' => $pol['ref_venta'],
                '!ip' => $pol['ip'],
                );
            $severity   = WATCHDOG_ALERT;
            $link       = NULL;
            if (_uc_pagosonline_is_confirmation()) {
                uc_order_comment_save($pol['order_id'], 0, t("Trying to pay order !order_id but the reference value (!ref_val) doesn't match. IP !ip", $variables));
                watchdog('PagosOnline', "Trying to pay order !order_id but the reference value (!ref_val) doesn't match. IP !ip", $variables, $severity, $link);
            }
            else {
                drupal_set_message(t("Trying to pay order !order_id but the reference value (!ref_val) doesn't match.", $variables), 'error');
            }
        }
        if (_uc_pagosonline_is_confirmation() && !in_array($order->order_status, array('in_checkout', 'post_checkout'))) {
            $variables  = array(
                '!order_id' => $pol['order_id'],
                '!ip' => $pol['ip'],
                );
            $severity   = WATCHDOG_ALERT;
            $link       = NULL;
            uc_order_comment_save($pol['order_id'], 0, t("Trying to pay order !order_id, but it isn't in checkout nor processing. IP !ip", $variables));
            watchdog('PagosOnline', "Trying to pay order !order_id, but it isn't in checkout nor processing. IP !ip", $variables, $severity, $link);
        }
        switch ($pol['estado_pol']) {
            case 4: // Payment Received
            $variables  = array(
                '!order_id' => $pol['order_id'],
                '!ip' => $pol['ip'],
                );
            $severity   = WATCHDOG_INFO;
            $link       = NULL;
            if (_uc_pagosonline_is_confirmation()) {
                $tipo_medio_pago = uc_pagosonline_get_tipo_medio_pago($pol['tipo_medio_pago']);
                $medio_pago = uc_pagosonline_get_medio_pago($pol['medio_pago']);
                $comment = t("Order Completed.") . ' ' . t("PagosOnline # !ref_pol. !tipo_medio_pago (!medio_pago)", array('!ref_pol' => $pol['ref_pol'], '!tipo_medio_pago' => $tipo_medio_pago, '!medio_pago' => $medio_pago));
                uc_order_comment_save($pol['order_id'], 0, t("Order !order_id. Payment Received. IP !ip", $variables));
                uc_order_comment_save($pol['order_id'], 0, _uc_pagosonline_format_request($pol));
                uc_order_comment_save($pol['order_id'], 0, $comment, 'order');
                watchdog('PagosOnline', "Order !order_id. Payment Received. IP !ip", $variables, $severity, $link);
                uc_payment_enter($order->order_id, 'PagosOnline', $pol['valor'], 0, NULL, $comment);
                uc_cart_empty($pol['cart_id']);
                uc_cart_complete_sale($order, FALSE);
            }
            else {
                drupal_set_message(t("Order !order_id completed.", $variables));
                $messages['uc_msg_order_submit_format'] = variable_get('uc_msg_order_submit', uc_get_message('completion_message'));
                if (user_is_logged_in()) {
                    $message_type = 'logged_in';
                }
                else {
                    $message_type = 'existing_user';
                }
                $messages['uc_msg_order_' . $message_type . '_format'] = variable_get('uc_msg_order_' . $message_type, uc_get_message('completion_' . $message_type));
            }

            break;
            case 5: // Canceled
            case 8:
            case 9:
            $variables  = array(
                '!order_id' => $pol['order_id'],
                '!ip' => $pol['ip'],
                );
            $severity   = WATCHDOG_WARNING;
            $link       = NULL;
            if (_uc_pagosonline_is_confirmation()) {
                $tipo_medio_pago = uc_pagosonline_get_tipo_medio_pago($pol['tipo_medio_pago']);
                $medio_pago = uc_pagosonline_get_medio_pago($pol['medio_pago']);
                $comment = t("Order Canceled.") . ' ' . t("PagosOnline # !ref_pol. !tipo_medio_pago (!medio_pago)", array('!ref_pol' => $pol['ref_pol'], '!tipo_medio_pago' => $tipo_medio_pago, '!medio_pago' => $medio_pago));
                uc_order_update_status($pol['order_id'], 'canceled');
                uc_order_comment_save($pol['order_id'], 0, _uc_pagosonline_format_request($pol));
                uc_order_comment_save($pol['order_id'], 0, $comment, 'order', 'canceled');
                watchdog('PagosOnline', "Order !order_id canceled. IP !ip", $variables, $severity, $link);
            }
            else {
                drupal_set_message(t("Order !order_id canceled.", $variables), 'warning');
            }
            break;
            case 6: // Rejected
            $variables  = array('!order_id' => $pol['order_id'], '!ip' => $pol['ip']);
            $severity   = WATCHDOG_WARNING;
            $link       = NULL;
            if (_uc_pagosonline_is_confirmation()) {
                $tipo_medio_pago = uc_pagosonline_get_tipo_medio_pago($pol['tipo_medio_pago']);
                $medio_pago = uc_pagosonline_get_medio_pago($pol['medio_pago']);
                $comment = t("Order Rejected.") . ' ' . t("PagosOnline # !ref_pol. !tipo_medio_pago (!medio_pago)", array('!ref_pol' => $pol['ref_pol'], '!tipo_medio_pago' => $tipo_medio_pago, '!medio_pago' => $medio_pago));
                uc_order_update_status($pol['order_id'], 'canceled');
                uc_order_comment_save($pol['order_id'], 0, _uc_pagosonline_format_request($pol));
                uc_order_comment_save($pol['order_id'], 0, $comment, 'order', 'canceled');
                watchdog('PagosOnline', "Order !order_id rejected. IP !ip", $variables, $severity, $link);
            }
            else {
                drupal_set_message(t("Order !order_id rejected.", $variables), 'warning');
            }
            break;
            case 7: // Pending or Processing
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            $variables  = array(
                '!order_id' => $pol['order_id'],
                '!ip' => $pol['ip'],
                );
            $severity   = WATCHDOG_WARNING;
            $link       = NULL;
            if (in_array($order->order_status, array('payment_received', 'completed'))) {
                uc_order_comment_save($pol['order_id'], 0, _uc_pagosonline_format_request($pol));
                watchdog('PagosOnline', "Order !order_id completed. IP !ip", $variables, WATCHDOG_ALERT, $link);
            }
            elseif (in_array($order->order_status, array('pending', 'processing'))) {
                uc_order_comment_save($pol['order_id'], 0, _uc_pagosonline_format_request($pol));
                watchdog('PagosOnline', "Order !order_id is pending or in process. IP !ip", $variables, WATCHDOG_ALERT, $link);
            }
            else {
                $tipo_medio_pago = uc_pagosonline_get_tipo_medio_pago($pol['tipo_medio_pago']);
                $medio_pago = uc_pagosonline_get_medio_pago($pol['medio_pago']);
                $comment = t("Order pending or in process to be approved.") . ' ' . t("PagosOnline # !ref_pol. !tipo_medio_pago (!medio_pago)", array('!ref_pol' => $pol['ref_pol'], '!tipo_medio_pago' => $tipo_medio_pago, '!medio_pago' => $medio_pago));
                uc_order_update_status($pol['order_id'], 'pending');
                uc_order_comment_save($pol['order_id'], 0, _uc_pagosonline_format_request($pol));
                uc_order_comment_save($pol['order_id'], 0, $comment, 'order');
                uc_cart_empty($pol['cart_id']);
                watchdog('PagosOnline', "Order !order_id is pending or in process. IP !ip", $variables, $severity, $link);
            }
            if (_uc_pagosonline_is_complete()) {
                if (in_array($order->order_status, array('payment_received', 'completed'))) {
                    drupal_set_message(t("Order !order_id completed.", $variables));
                }
                else {
                    drupal_set_message(t("Order !order_id is pending or in process.", $variables), 'warning');
                }
            }
            break;
            default:
            $variables  = array(
                '!order_id' => $pol['order_id'],
                '!ip' => $pol['ip'],
                '!message' => $pol['mensaje'],
                '!estado_pol_k' => $pol['estado_pol'],
                '!estado_pol' => uc_pagosonline_get_estado_pol($pol['estado_pol']),
                '!codigo_respuesta_pol_k' => $pol['codigo_respuesta_pol'],
                '!codigo_respuesta_pol' => uc_pagosonline_get_codigo_respuesta_pol($pol['codigo_respuesta_pol']),
                '!helppage' => l('help', variable_get('uc_store_help_page', '')),
                );
            $severity   = WATCHDOG_ALERT;
            $link       = NULL;
            if (_uc_pagosonline_is_confirmation()) {
                uc_order_comment_save($pol['order_id'], 0, t("Order !order_id. Something is wrong with your transaction, please check the store !help page.", $variables));
                uc_order_comment_save($pol['order_id'], 0, _uc_pagosonline_format_request($pol));
                watchdog('PagosOnline', "Order !order_id. Something is wrong with your transaction, please check the store !help page.", $variables, $severity, $link);
            }
            else {
                drupal_set_message(t("Order !order_id. Something is wrong with your transaction, please check the store !help page.", $variables), 'error');
            }
            break;
        }
    }
    if (_uc_pagosonline_is_complete()) {
        $messages['uc_msg_continue_shopping_format'] = variable_get('uc_msg_continue_shopping', uc_get_message('continue_shopping'));
        $output_message = '';
        foreach ($messages as $format => $message) {
            $message = token_replace($message, array('global' => NULL, 'order' => $order));
            $message = check_markup($message, variable_get($format, 'FILTER_FORMAT_DEFAULT'), $langcode = '' /* TODO Set this variable. */, FALSE);
            $output_message .= $message;
        }
        return $output_message;
    }
}

function _uc_pagosonline_format_request($pol) {
    $output = t('PagosOnline answer:');
    $output .= '<ul>';
    $output .= '<li>' . t('Reference # (!key): !value', array('!key' => 'ref_pol', '!value' => $pol['ref_pol'])) . '</li>';
    $output .= '<li>' . t('Transaction status (!key): !value !info', array('!key' => 'estado_pol', '!value' => $pol['estado_pol'], '!info' => uc_pagosonline_get_estado_pol($pol['estado_pol']))) . '</li>';
    $output .= '<li>' . t('Response code (!key): !value !info', array('!key' => 'codigo_respuesta_pol', '!value' => $pol['codigo_respuesta_pol'], '!info' => uc_pagosonline_get_codigo_respuesta_pol($pol['codigo_respuesta_pol']))) . '</li>';
    if ($pol['mensaje']) {
        $output .= '<li>' . t('Message (!key): !value', array('!key' => 'mensaje', '!value' => $pol['mensaje'])) . '</li>';
    }
    if ($pol['ip']) {
        $output .= '<li>' . t('IP (!key): !value', array('!key' => 'ip', '!value' => $pol['ip'])) . '</li>';
    }
    $output .= '</ul>';
    return $output;
}

