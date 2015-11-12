<?php

function commerce_przelewy24_verify()
{
    if(isset($_POST))
    {
        if(isset($_POST['p24_id_sprzedawcy']) and isset($_POST['p24_session_id']) 
                and isset($_POST['p24_order_id']) and isset($_POST['p24_order_id_full'])
                and isset($_POST['p24_kwota']))
        {
            $s_session_id=$_POST['p24_session_id'];
            $sa_post=explode('|',$s_session_id);

            $i_p24_order_id=preg_replace('/\D/','',$_POST['p24_order_id']);
            $i_order_id=preg_replace('/\D/','',$sa_post[0]);
            $i_amount=preg_replace('/\D/','',$_POST['p24_kwota']);
            $o_order=commerce_order_load($i_order_id);
            // do wyciagniecia kwoty
            $o_order_wrapped=entity_metadata_wrapper('commerce_order',$o_order);
            
            $s_url_fail="checkout/".$o_order->order_id."/payment/back/".$o_order->data['payment_redirect_key'];
            
            if(isset($_POST['p24_error_code']))
            {
                drupal_set_message(t('Transakcja nieudana.'));
                drupal_goto($s_url_fail);
            }
            
            $i_id_payment_method=$o_order->data['payment_method'];
            // wyciagniecie danych metody platnosci
            $a_payment_method=commerce_payment_method_instance_load($i_id_payment_method);
            
            if(isset($_POST['p24_crc']) and $a_payment_method['settings']['test']==0)
            {
                $s_crc=md5($s_session_id.'|'.$a_payment_method['settings']['id_sp']
                          .'|'.$i_amount.'|'.$a_payment_method['settings']['crc']);
                if($s_crc!=$_POST['p24_crc'])
                {
                    drupal_set_message(t('Transakcja nieudana.'));
                    drupal_goto(url($s_url_fail,array('absolute'=>true)));
                }
            }
            
            $i_amount_stored=$o_order_wrapped->commerce_order_total->amount->value();
            if($i_amount==$i_amount_stored)
            {
                if(empty($transaction)) //??
                {
                    $transaction=commerce_payment_transaction_new('przelewy24',$o_order->order_id);
                    $transaction->created=time();
                }
                $transaction->instance_id=$a_payment_method['instance_id'];

                $transaction->remote_id=$o_order->order_id;
                $transaction->amount=$i_amount;
                $transaction->currency_code='PL';
                $transaction->payload[REQUEST_TIME]=$_POST;
                
                if(p24ok($s_session_id,$i_p24_order_id
                        ,$a_payment_method['settings'],$i_amount_stored))
                {
                    $transaction->changed=time();

                    $transaction->message=t('Płatność poprawna');
                    $transaction->status=COMMERCE_PAYMENT_STATUS_SUCCESS;
                    $transaction->remote_status=$transaction->status;
                    commerce_checkout_complete($o_order);
                    commerce_payment_transaction_save($transaction);
                    drupal_goto(url('checkout/'.$o_order->order_id.'/payment/return/'.$o_order->data['payment_redirect_key']
                                ,array('absolute'=>true)));
                }
                else
                {
                    $transaction->changed=time();
                    $transaction->status=COMMERCE_PAYMENT_STATUS_FAILURE;
                    commerce_payment_transaction_save($transaction);
                    drupal_goto($s_url_fail);
                }
            }
        }
    }
}


function p24ok($p24_session_id,$p24_order_id,$a_settings,$i_amount)
{
    $i_p24_order_id=(int) $p24_order_id;

    $s_url='https://secure.przelewy24.pl';
    $P=array(); $RET = array();
    if($a_settings['test']==1)
    {
        $s_url='https://sandbox.przelewy24.pl';
    }
    else
    {
        $P[]=$a_settings['crc'];
    }

    $url=$s_url."/transakcja.php";
    $P[]= "p24_id_sprzedawcy=".$a_settings['id_sp'];
    $P[]= "p24_session_id=".$p24_session_id;
    $P[]= "p24_order_id=".$i_p24_order_id;
    $P[]= "p24_kwota=".$i_amount;

    $user_agent = "Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)";
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_POST,1);
    if(count($P)) curl_setopt($ch, CURLOPT_POSTFIELDS,join("&",$P));
    curl_setopt($ch, CURLOPT_URL,$url);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2);
    curl_setopt($ch, CURLOPT_USERAGENT, $user_agent);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    $result=curl_exec($ch);

    curl_close($ch);
    $T=explode(chr(13).chr(10),$result);
    $res = false;
    $RET=array();
    foreach($T as $line)
    {
        $line = ereg_replace("[\n\r]","",$line);
        if($line != "RESULT" and !$res) continue;
        if($res) $RET[] = $line;
        else $res = true;
    }

    if($RET[0]=='TRUE')
    {
        
    return true;
    }

return false;
}