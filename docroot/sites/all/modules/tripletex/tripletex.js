// $Id$
Drupal.behaviors.tripletex = function (context) {
  $('#edit-product-description', context).change(function () {
    // This function will get exceuted after the ajax request is completed successfully
    var productObjects = function(data) {
      // The data parameter is a JSON object. The “products” property is the list of products items that was returned from the server response to the ajax request.
      $('#edit-product-price').attr('value', data.product.priceIncVatCurrency);
    };
    $.ajax({
      type: 'GET',
      url: '/invoices/product/details/' + 'all', // Which url should be handle the ajax request. This is the url defined in the <a> html tag
      success: productObjects, // The js function that will be called upon success request
      dataType: 'json', //define the type of data that is going to get back from the server
      data: 'js=1' //Pass a key/value pair
    });
  });
  
  // Act when the field edit-product changes to load the corresponding price and product id
  $('#edit-product', context).change(function () {
    // This function will get exceuted after the ajax request is completed successfully
    var productObjects = function(data) {
      // The data parameter is a JSON object. The “products” property is the list of products 
      // items that was returned from the server response to the ajax request.
      $('#edit-price').attr('value', data.product.priceIncVatCurrency);
      $('#edit-product-id').attr('value', data.product.id);
      $('#edit-product-number').attr('value' .data.product.number);
      $('#edit-product-vat-type').attr('value' .data.product.vatType);
	  //$('p').text('ID:' + this.val());
    };
    $.ajax({
      type: 'GET',
      url: '/invoices/product/details/' + this.value, 
      success: productObjects, // The js function that will be called upon success request
      dataType: 'json', //define the type of data that is going to get back from the server
      data: 'js=1' //Pass a key/value pair
    });
  });
  
  // Act when name field is changed to load via AJAX the corresponding address details and customer id
  $('#edit-name', context).change(function () {
    // This function will get exceuted after the ajax request is completed successfully
    var customerObjects = function(data) {
      // The data parameter is a JSON object. The “products” property is the list of products 
      // items that was returned from the server response to the ajax request.
      $('#edit-address').attr('value', data.customer.address1);
      $('#edit-city').attr('value', data.customer.city);
      $('#edit-postcode').attr('value', data.customer.postalcode);
    };
    $.ajax({
      type: 'GET',
      url: '/invoices/customer/details/' + this.value,
      success: customerObjects, // The js function that will be called upon success request
      dataType: 'json', //define the type of data that is going to get back from the server
      data: 'js=1' //Pass a key/value pair
    });
  });

};
