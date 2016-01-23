/**
 * @file
 * Novalnet payment method module
 * This module is used for real time processing of
 * Novalnet transaction of customers.
 *
 * Copyright (c) Novalnet AG
 *
 * Released under the GNU General Public License
 * This free contribution made by request.
 * If you have found this script useful a small
 * recommendation as well as a comment on merchant form
 * would be greatly appreciated.
 *
 * Script : sepa.js
 *
 */
jQuery(document).ready(function(){
                sepa_refill_cal();
                jQuery( document ).ajaxComplete(function( event, xhr, settings ) {
                    jQuery("#novalnet_sepa_confirm_id")
                    sepa_refill_cal();
                });
        jQuery('#edit-continue').click(function(event){
           if(jQuery("#novalnet_sepa_confirm_id").is(":checked") == false && jQuery("#sepa_cardholder").val() !=undefined) {
            event.preventDefault();
                        alert(jQuery("#confirm_iban_bic_msg").val());
            jQuery("input:disabled").remove();
            jQuery('#edit-continue').css('display','inline');
            jQuery('.checkout-processing').css('display','none');
           }
        });
    jQuery('#novalnet_sepa_confirm_id').attr('checked',false);
});

function show_overlay(element){

            if(document.getElementById('novalnet_sepa_confirm_id').checked == true){
                jQuery('#novalnet_sepa_confirm_id').attr('disabled','disabled');
            if(document.getElementById('sepa_cardholder').value == '' || document.getElementById('sepa_iban').value == '' || document.getElementById('sepa_country').value == ''){
                alert(jQuery("#invalid_error_msg").val());
                hide_bank_details();
                jQuery("#novalnet_sepa_confirm_id").removeAttr('disabled');
                document.getElementById('novalnet_sepa_confirm_id').checked = false;
                return false;
            }
            var vendor_id       = document.getElementById('vendor_id').value;
            var vendor_authcode = document.getElementById('vendor_authcode').value;
            var bank_country   = document.getElementById('sepa_country').value;
            var account_holder = remove_unwanted_special_chars(document.getElementById('sepa_cardholder').value);
            var bank_account   = document.getElementById('sepa_iban').value;
            var bank_code      = document.getElementById('sepa_bic').value;
            var unique_id      = document.getElementById('sepa_unique_id').value;
            account_holder = jQuery.trim(account_holder);
            if (vendor_id == '' || vendor_authcode == '') {
                alert( jQuery( '#invalid_error_msg' ).val() );
                hide_bank_details();
                return false;
            }
            if(bank_country == 'DE' && bank_code == '' && isNaN(bank_account)){
                bank_code = '123456';
            }
            else if(bank_country == 'DE' && !isNaN(bank_code) && isNaN(bank_account)){
                alert( jQuery( '#invalid_error_msg' ).val() );
                hide_bank_details();
                return false;
            }
            if(bank_country != 'DE' && (bank_code == '' || ((!isNaN(bank_account) && isNaN(bank_code)) || (isNaN(bank_account) && !isNaN(bank_code)))) ){
                alert(jQuery( '#invalid_error_msg' ).val());
                hide_bank_details();
                return false;
            }
            else if((bank_country == 'DE' && (!isNaN(bank_account) && (bank_code == '' || isNaN(bank_code))))){
            alert(jQuery( '#invalid_error_msg' ).val() );
                hide_bank_details();
                return false;
            }

            if(is_numeric(bank_account) && is_numeric(bank_code)){
                    var iban_bic = {'account_holder':account_holder,'bank_account':bank_account,'bank_code':bank_code,'vendor_id':vendor_id,'vendor_authcode':vendor_authcode,'bank_country':bank_country,'unique_id':unique_id,'get_iban_bic':1}
                    iban_bic = jQuery.param(iban_bic);
                    sent_xdomainreq_sepa( iban_bic, 'nnsepa_iban' , null);

            }
            else {
                    var hash_gen =
                    {'account_holder':account_holder,'bank_account':'','bank_code':'','vendor_id':vendor_id,'vendor_authcode':vendor_authcode,'bank_country':bank_country,'unique_id':unique_id,'sepa_data_approved':1,'mandate_data_req':1,'iban':bank_account,'bic':bank_code}
                    hash_gen = jQuery.param(hash_gen);
                    sent_xdomainreq_sepa( hash_gen, 'nnsepa_hash' , null);
            }

            document.getElementById('sepa_bic_gen').value = bank_code;
            document.getElementById('sepa_iban_gen').value = bank_account;
            }else {
                close_mandate_overlay_on_cancel();
            }
    }

    function is_numeric(ele){
        return (/^[0-9]+$/.test(ele));
   }

    function close_mandate_overlay_on_cancel(){
        document.onkeydown = function(evt) {
        return true;
        };
                jQuery('#sepa_mandate_overlay_block').hide(60);
                jQuery('.bgCover').css( {display:'none'} );
       document.getElementById('novalnet_sepa_confirm_id').checked = false;
       document.getElementById('nn_sepa_ibanbic_confirm_id').value = 0;
       jQuery('#novalnet_sepa_confirm_id').removeAttr('disabled');
       hide_bank_details();
    }

    function confirm_mandate_overlay(){
        document.onkeydown = function(evt) {
        return true;
        };
       document.getElementById('novalnet_sepa_confirm_id').checked = true;
       document.getElementById('nn_sepa_ibanbic_confirm_id').value = 1;
       jQuery('#novalnet_sepa_confirm_id').removeAttr('disabled');
                jQuery('#sepa_mandate_overlay_block').hide(60);
                jQuery('.bgCover').css( {display:'none'} );
       jQuery('#loading_img').css('display','none');
    }

    function get_bgcover() {
             jQuery('.bgCover').css({
                    display:'block',
                        width: '100%',
                    height: jQuery(document).height()
                });
                jQuery('.bgCover').css({opacity:0.5});

                if(isNaN(jQuery('#sepa_iban_gen').val())) {
                    template_iban = jQuery('#sepa_iban_gen').val();
                } else {
                    template_iban = jQuery('#sepa_iban').val();
                }
                if(isNaN(jQuery('#sepa_bic_gen').val())) {
                    template_bic = jQuery('#sepa_bic_gen').val();
                } else {
                    template_bic = jQuery('#sepa_bic').val();
                }
                jQuery('#mandate_overlay_country').html(jQuery("#sepa_country").find("option[value='" + jQuery("#sepa_country").val() + "']").text());
                jQuery('#sepa_overlay_iban_span').html(template_iban);
                if(template_bic != '')
                {
                    jQuery('#sepa_overlay_bic_span').html(template_bic);
                    jQuery('#nn_sepa_overlay_bic_tr').show(60);
                } else {
                    jQuery('#sepa_overlay_bic_span').html('');
                    document.getElementById('nn_sepa_overlay_bic_tr').style.display='none';
                }
                jQuery('#sepa_overlay_payee_span').html('Novalnet AG');
                jQuery('#sepa_overlay_creditoridentificationnumber_span').html('DE53ZZZ00000004253');
                jQuery('#overlay_card_holder_name').html(remove_unwanted_special_chars(document.getElementById('sepa_cardholder').value));
                jQuery('#overlay_card_holder_sign').html(remove_unwanted_special_chars(document.getElementById('sepa_cardholder').value));
                jQuery('#sepa_overlay_mandatedate_span').html(normalize_date(jQuery('#sepa_mandate_date').val()));
                jQuery('#sepa_overlay_mandatereference_span').html(jQuery('#sepa_mandate_ref').val());
                jQuery('#sepa_mandate_overlay_block').css({ display:'block', position:'fixed' });
                jQuery('#sepa_mandate_overlay_block').attr('tabIndex',-1).focus();


                     if(jQuery(window).width() < 650)
                    {
                        jQuery('#sepa_mandate_overlay_block').css({left:'25%',top:'10%',width:0,height:0}).animate( {left:'25%',top:'1%',width:'55%',height:(jQuery(window).height()-10)} );
                        jQuery('#overlay_window_block_body').css({'height':(jQuery(window).height()-95)});
                    } else {
                        jQuery('#sepa_mandate_overlay_block').css( {left:'25%',top:'10%',width:'45%',height:(490)} );
                        jQuery('#overlay_window_block_body').css({'height':(400)});
                    }
  return true;
    }

    function hide_bank_details() {
        jQuery('#nn_sepa_bic_val').hide();
        jQuery('#nn_sepa_iban_val').hide();
        jQuery('#novalnet_sepa_confirm_id').attr('checked',false);
        jQuery("#novalnet_sepa_confirm_id").removeAttr('disabled');
    }

    function sent_xdomainreq_sepa(qryString, ptype, from_iban){
    document.getElementById('nnloader').style.display = 'block';
    var nnurl = (document.location.protocol == 'http:') ? "http://payport.novalnet.de/sepa_iban" : "https://payport.novalnet.de/sepa_iban";
        if ('XDomainRequest' in window && window.XDomainRequest !== null) {
            var xdr = new XDomainRequest();
            xdr.open('POST', nnurl);
            xdr.onload = function () {
                return check_result_sepa(this.responseText, ptype, qryString , from_iban);
            }
            xdr.onerror = function() {
                _result = false;
            };
            xdr.send(qryString);
        }
        else{
            var xmlhttp = (window.XMLHttpRequest) ? new XMLHttpRequest() : new ActiveXObject("Microsoft.XMLHTTP");
            xmlhttp.onreadystatechange=function(){
                if (xmlhttp.readyState==4 && xmlhttp.status==200){
                    return check_result_sepa( xmlhttp.responseText, ptype, qryString , from_iban);
                }
            }
            xmlhttp.open("POST",nnurl,true);
            xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
            xmlhttp.send(qryString);
        }
    }


    function check_result_sepa(response, ptype, qryString, from_iban){
        var data = jQuery.parseJSON(response);
        document.getElementById('nnloader').style.display = 'none';
        if(data.hash_result != 'success'){
            alert(data.hash_result);
            return false;
        }
      switch (ptype){
        case 'nnsepa_iban':
            if(data.IBAN =='' || data.BIC == ''){
                alert( jQuery( '#invalid_error_msg').val() );
                hide_bank_details();
                return false;
            }
            document.getElementById('sepa_bic_gen').value = data.BIC;
            document.getElementById('sepa_iban_gen').value = data.IBAN;
            jQuery("#nn_sepa_iban_val").text('IBAN : '+ data.IBAN);
            jQuery("#nn_sepa_bic_val").text('BIC : '+ data.BIC);
            jQuery('#nn_sepa_bic_val').show();
            jQuery('#nn_sepa_iban_val').show();

            hash_data = qryString.replace('&get_iban_bic=1','') + '&sepa_data_approved=1&mandate_data_req=1&iban=' + data.IBAN + '&bic=' + data.BIC ;
            sent_xdomainreq_sepa(hash_data, 'nnsepa_hash', null);
          break;
        case 'nnsepa_hash':
                document.getElementById('sepa_mandate_ref').value = data.mandate_ref;
                document.getElementById('nn_sepa_panhash').value = data.sepa_hash;
                document.getElementById('sepa_mandate_date').value = data.mandate_date;
                                get_bgcover();

          break;
        case 'nnsepa_refil':
           jQuery('#nn_sepa_hash_generated').val(1);
            jQuery('#loading-img').hide();
            var hash_string = data.hash_string;
            var acc_hold = hash_string.match('account_holder=(.*)&bank_code');
            var holder='';
            if(acc_hold != null && acc_hold[1] != undefined) holder = acc_hold[1];
            var params = data.hash_string.split("&");
            jQuery('#nnsepa_mandate_ref').html( data.mandate_ref );
            jQuery( '#nnsepa_owner' ).val( jQuery( '#nnsepa_holder_name').val() );
            jQuery.each( params, function( i, keyVal ){
            temp = keyVal.split('=');
            if (jQuery('#sepa_cardholder').val() != undefined)
                document.getElementById('sepa_cardholder').value = remove_unwanted_special_chars(holder);
            switch (temp[0]){
              case 'bank_account' :
                    jQuery('#sepa_iban').val( temp[1] );
                break
              case 'bank_code' :
                    jQuery('#sepa_bic').val( temp[1] );
                break
              case 'bank_country' :
                    jQuery('#sepa_country').val( temp[1]);
                break;
              case 'iban' :
                    jQuery('#sepa_iban').val( temp[1] );
                break;
              case 'bic' :
                     if(temp[1] != '123456')
                    {
                       jQuery('#sepa_bic').val( temp[1] );
                    }
                break;
            }
          });
          if(jQuery( '#frm_iban').html() != ''){
            jQuery('#nnsepa_iban_div').show();
            jQuery('#nnsepa_bic_div').show();
          }
      }
    }


function disable_background_events(){
    document.onkeydown = function(evt) {
        var charCode = (evt.which) ? evt.which : evt.keyCode;
        if ((evt.ctrlKey == true && charCode == 114)|| charCode == 116) {
          return true;
        }
        return false;
    };
}

function on_change(){
jQuery('#novalnet_sepa_confirm_id').attr('checked',false);
jQuery('#nn_sepa_panhash').val ='';
hide_bank_details();
jQuery('#sepa_mandate_overlay_block').css({ display:'none'});
jQuery('.bgCover').css({ display:'none'});
}
function remove_unwanted_special_chars(input_val) {
    if( input_val != 'undefined' || input_val != ''){
        return input_val.replace(/[\/\\|\]\[|#,+()$~%.":`'@~;*?<>!^{}=_-]/g,'');
    }
 }
function normalize_date(input) {
  if(input != 'undefined' && input != '') {
    var parts = input.split('-');

    return (parts[2] < 10 ? '0' : '') + Number(parts[2]) + '.'
      + (parts[1] < 10 ? '0' : '') + Number(parts[1]) + '.'
      + parseInt(parts[0]);
  }
}
function sepa_refill_cal() {
  if(jQuery('#refillpanhash').val() == undefined)
        {  return false;}
        if (jQuery('#is_refill').val() == '1') {
            var vendor_id       = jQuery('#vendor_id').val();
            var vendor_authcode = jQuery('#vendor_authcode').val();
            var refillpanhash = jQuery('#refillpanhash').val();
            var refilluniq = jQuery('#sepa_unique_id').val();
            var refill_params = "vendor_id="+vendor_id+"&vendor_authcode="+vendor_authcode+"&unique_id="+refilluniq+"&sepa_data_approved=1&mandate_data_req=1&sepa_hash="+refillpanhash;
            if(refillpanhash != '' && refillpanhash !=undefined && refilluniq != '' && refilluniq !=undefined) {
                sent_xdomainreq_sepa( refill_params , 'nnsepa_refil' , null);
            }
        }else{
            jQuery('#novalnet_sepa_confirm_id').attr('checked',false);
            jQuery('#nn_sepa_panhash').attr('value','');
            jQuery('#sepa_bic').attr('value','');
            jQuery('#sepa_iban').attr('value','');
            jQuery('#sepa_cardholder').attr('value',jQuery('#nndefault_holder').val());
            jQuery('#sepa_country').attr('value',jQuery('#nndefault_country').val());
            document.getElementById('novalnet_sepa_confirm_id').checked = false;
        }

}

function key_lock(evt) {
    var charCode = (evt.which) ? evt.which : evt.keyCode
    if ((evt.ctrlKey == true && charCode == 114)|| charCode == 116) {
      return true;
    }
    return false;
}
function ibanbic_validate(event){
    var keycode = ('which' in event) ? event.which : event.keyCode;
    var reg = /^(?:[A-Za-z0-9]+$)/;
    if(event.target.id == 'sepa_cardholder') var reg = /^(?:[A-Za-z0-9&\s]+$)/;
    return (reg.test(String.fromCharCode(keycode)) || keycode == 0 || keycode == 8 || (event.ctrlKey == true && keycode == 114))? true : false;
 }
