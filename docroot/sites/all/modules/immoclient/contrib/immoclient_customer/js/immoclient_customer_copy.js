(function ($) {

/**
 * Copy from immobilie adress field to customer adress field
 */
Drupal.behaviors.PersonCopy = {
  attach: function (context) {    
    // Use the radiobutton id in a fieldset to identify if copy is set to yes
    $('fieldset #edit-field-seller-und-form-customer-adress #edit-field-seller-und-form-field-customer-copy', context).click(function () {
        // take the value from the copy radio field
       var value = $("input[name='field_seller[und][form][field_customer_copy][und]']:checked").val();
        if (value === "YES"){
         var street = $("input[name='field_addresse[und][0][street]']").val();
         var street_seller = $("input[name='field_seller[und][form][field_customer_strasse_hnr][und][0][value]']").val();
         if(street_seller === ''){
            $("input[name='field_seller[und][form][field_customer_strasse_hnr][und][0][value]']").val(street);
         }
         var city =$("input[name='field_addresse[und][0][city]']").val();
         var city_seller = $("input[name='field_seller[und][form][field_customer_stadt][und][0][value]']").val();
         if(city_seller === ''){
        $("input[name='field_seller[und][form][field_customer_stadt][und][0][value]']").val(city);
         }
         var plz =$("input[name='field_addresse[und][0][postal_code]']").val();
         var plz_seller = $("input[name='field_seller[und][form][field_customer_plz][und][0][value]']").val();
         if(plz_seller === ''){
        $("input[name='field_seller[und][form][field_customer_plz][und][0][value]']").val(plz);
         }
                       
         var country = $("select[name='field_addresse[und][0][country]']").val().toUpperCase();
         var country_seller = $("input[name='field_seller[und][form][field_customer_land][und][0][value]']").val();
         console.log(country_seller);
            if(country_seller === ''){
             $("input[name='field_seller[und][form][field_customer_land][und][0][value]']").val(country);
         }
        }
        if (value === 'NO'){
            $("input[name='field_seller[und][form][field_customer_strasse_hnr][und][0][value]']").val("");
            $("input[name='field_seller[und][form][field_customer_stadt][und][0][value]']").val("");
            $("input[name='field_seller[und][form][field_customer_plz][und][0][value]']").val("");
            $("input[name='field_seller[und][form][field_customer_land][und][0][value]']").val("");
        }
     });    
    }
  
};

})(jQuery);


