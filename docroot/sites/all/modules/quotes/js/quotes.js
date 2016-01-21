/**
* @file
* Quotes javascript files.
*
* Javascript for changing displays of quotes of pages containing quotes.
*/

(function ($) {
  function quotes_more_handler(event) {
    if ($("input[name=block_type]:checked").val() == '0') {
      $("input[name=block_more]").parents("div.quotes_block_more").show();
   }
   else {
     $("input[name=block_more]").parents("div.quotes_block_more").hide();
   }
  }

   // Run the javascript on page load.
   $(document).ready(function () {
   // On page load, determine the default settings.
   quotes_more_handler();

   // Bind the functions to click events.
   $("input[name=block_type]").bind("click", quotes_more_handler);
  });
})(jQuery);