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
 * Script : cc.js
 *
 */
jQuery(document).ready(function(){

     refill_cal();
            jQuery( document ).ajaxComplete(function( event, xhr, settings ) {
        refill_cal();
    });
    jQuery('#edit-continue').click(function(event){
       if(jQuery("#nn_cc_number").val() != undefined) {
        event.preventDefault();
        pan_hash_generate();
       }
    });
});

function show_cchint(){
    x=jQuery('#cvc-hint').position();
    var itop = x.top-100;
        jQuery('#cvc-hint-img').css({'display':'block'});
        jQuery('#cvc-hint-img img').css({'top':itop+'px','display':'block'});
    jQuery('#cvc-hint-img img').css({'left':140+'px','position':'absolute'});

}

function hide_cchint(){
    jQuery('#cvc-hint-img').hide();
}

function refill_cal(){
    if(jQuery('#refillhash').val() == undefined)
        {  return false;}
    if (jQuery('#is_refill').val() == '1') {
        var refillhash = jQuery('#refillhash').val();
        var refilluniq = jQuery('#nn_cc_unique_id').val();
        var cc_vendor_id = jQuery('#nn_vendor_id').val();
        var cc_authcode = jQuery('#nn_authcode').val();
        var refillparam = "pan_hash="+refillhash+"&unique_id="+refilluniq+"&vendor_id="+cc_vendor_id+"&vendor_authcode="+cc_authcode;
        if(refillhash!= '' && refilluniq!= '' && refillhash!=undefined && refilluniq!=undefined) {
            sent_xdomainreq(refillparam, 'ccrefill', null)
        }
    }else{
            jQuery('#nn_cc_cardtype').attr('value','');
            jQuery('#nn_cc_number').attr('value','');
            jQuery('#nn_cc_expmonth').attr('value','');
            jQuery('#nn_cc_expyear').attr('value','');
            jQuery('#nn_cc_cvv').attr('value','');
            jQuery('#nn_cc_cardholder').attr('value',jQuery('#nndefault_holder').val());
        }

}

function ccnumber_validate(event){
    var keycode = ('which' in event) ? event.which : event.keyCode;
    var reg = /^(?:[0-9\s]+$)/;
    return (reg.test(String.fromCharCode(keycode)) || keycode == 0 || keycode == 8 || (event.ctrlKey == true && keycode == 114))? true : false;
 }


function pan_hash_generate(){

    var cc_type = jQuery('#nn_cc_cardtype').val();
    var cc_holder = remove_unwanted_special_chars(jQuery('#nn_cc_cardholder').val());
    var cc_number = get_numbers(jQuery('#nn_cc_number').val());
    var cc_expire = jQuery('#nn_cc_expmonth').val();
    var cc_expire_yr = jQuery('#nn_cc_expyear').val();
    var cc_cvc = jQuery('#nn_cc_cvv').val();
    var cc_vendor_id = jQuery('#nn_vendor_id').val();
    var cc_authcode = jQuery('#nn_authcode').val();
    var cc_unique_id = jQuery('#nn_cc_unique_id').val();
     cc_holder = jQuery.trim(cc_holder);
    var dObj = new Date();
    var cYear = dObj.getFullYear();
    var cMonth = dObj.getMonth();
    if (cc_expire == 0 || cc_expire_yr==0 || (cc_expire_yr == cYear && cc_expire < (cMonth + 1))) {
        alert(jQuery('#invalid_error_msg').val());
        jQuery("input:disabled").remove();
        jQuery('#edit-continue').css('display','inline');
        jQuery('.checkout-processing').css('display','none');
        return false;
    }
    var param_arr = [cc_type,cc_holder,cc_number,cc_expire,cc_expire_yr,cc_cvc,cc_vendor_id,cc_authcode,cc_unique_id];

    if(jQuery.inArray( "", param_arr ) == -1 && jQuery.inArray( undefined, param_arr ) == -1) {
    var panhash_params = {'noval_cc_exp_month':cc_expire,'noval_cc_exp_year':cc_expire_yr,'noval_cc_holder':cc_holder,'noval_cc_no':cc_number,'noval_cc_type':cc_type,'unique_id':cc_unique_id,'vendor_authcode':cc_authcode,'vendor_id':cc_vendor_id};
    panhash_params = jQuery.param(panhash_params);
    document.getElementById('nnloader').style.display = 'block';
    sent_xdomainreq(panhash_params, 'ccpanhash', null);
    }
    else{
    alert(jQuery('#invalid_error_msg').val());
    jQuery("input:disabled").remove();
    jQuery('#edit-continue').css('display','inline');
    jQuery('.checkout-processing').css('display','none');
    }
}



function sent_xdomainreq(qryString, ptype, from_iban){

    var nnurl_cc = (document.location.protocol == 'http:')? "http://payport.novalnet.de/payport_cc_pci" : "https://payport.novalnet.de/payport_cc_pci";
            if ('XDomainRequest' in window && window.XDomainRequest !== null) {
                    var xdr = new XDomainRequest();
                    xdr.open('POST', nnurl_cc);
                    xdr.onload = function () {
                            return check_result(this.responseText, ptype, qryString , from_iban);
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
                                    return check_result( xmlhttp.responseText, ptype, qryString , from_iban);
                            }
                    }
                    xmlhttp.open("POST",nnurl_cc,true);
                    xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
                    xmlhttp.send(qryString);
            }
    }


    function check_result(response, ptype, qryString, from_iban){
                document.getElementById('nnloader').style.display = 'none';
        var data = jQuery.parseJSON(response);
        if(data.hash_result != 'success') {
            alert(data.hash_result);
            return false;
        }
      switch (ptype){
        case 'ccpanhash':
            jQuery('#nn_cc_panhash').val(data.pan_hash);
            jQuery('#commerce-checkout-form-review').submit();
          break;
        case 'ccrefill':
        jQuery('#nn_cc_cvv').attr('value','');
        var acc_hold = data.hash_string.match('cc_holder=(.*)&cc_exp_year');
        var holder='';
        if(acc_hold != null && acc_hold[1] != undefined) holder = acc_hold[1];
        var params = data.hash_string.split("&");
        jQuery.each( params, function( i, keyVal ){
        temp = keyVal.split('=');
        if (jQuery('#nn_cc_cardholder').val() != undefined)
        document.getElementById('nn_cc_cardholder').value = remove_unwanted_special_chars(holder);
        switch (temp[0]){
          case 'cc_exp_year' :
              jQuery('#nn_cc_expyear').val( temp[1] );
            break
          case 'cc_exp_month' :
                            temp[1] = (temp[1]<10)? '0'+temp[1]:temp[1];
            jQuery('#nn_cc_expmonth').val(temp[1]);
            break
          case 'cc_no' :
            jQuery('#nn_cc_number').val( temp[1]);
            break;
          case 'cc_type' :
            jQuery('#nn_cc_cardtype').val( temp[1]);
            break;
        }
      });
      }
    }

function remove_unwanted_special_chars(input_val) {
    if( input_val != 'undefined' || input_val != ''){
        return input_val.replace(/[\/\\|\]\[|#,+()$~%.":`'@~;*?<>!^{}=_-]/g,'');
    }
 }
 function get_numbers(input_val) {
   var input_val = input_val.replace(/^\s+|\s+$/g, '');
  return input_val.replace(/[^0-9]/g,'');
}
function accholder_validate(eve, key){
    var keycode = ('which' in eve) ? eve.which : eve.keyCode;
    var reg = /^(?:[A-Za-z0-9&\s]+$)/;
    if (key == 'cvc') reg = /^(?:[0-9]+$)/;
    return (reg.test(String.fromCharCode(keycode)) || keycode == 0 || keycode == 8 || (eve.ctrlKey == true && keycode == 114))? true : false;
}
