(function ($) {
  Drupal.behaviors.loadingPages = {
    attach: function(context) {
      $('.show-loading').click(function(){
        $('#ebooks-loading-panel').css('display', 'block');
      });

      // See if Drupal is using clean URLs or not.
    	var ajaxUrlModel;
    	if (location.href.indexOf("?q=") == -1) {
    		ajaxUrlModel = "%s/search";
    	}
    	else {
    		ajaxUrlModel = "?q=ebooks/%s/search";
    	}

      var overdrive = $('#ebooks-overdrive');
      if (overdrive.length > 0) {
         var search_id     = $('#ebooks-last-search');
         var ebooks_search = search_id.attr('value');
         var only_available = $('#edit-ebooks-available').attr('value');
         var page_overdrive = '1';
         if (overdrive.html().trim() != 'No results were found.') { // If we've already got some results on the page, let's look for some more.
  	       AjaxOverdrive();
         }
      }

      function AjaxOverdrive() {
        $('.loading-overdrive').appendTo(overdrive);
        $('.loading-overdrive').css('display', 'block');
        var searches = {
          ebooks_search: ebooks_search,
          page_overdrive: page_overdrive,
          only_available: only_available
        };
        $.ajax({
          url:     ajaxUrlModel.replace("%s", "overdrive"),
          type:    'POST',
          success: overdriveLoading,
          data:    searches
        });
      }

      function overdriveLoading(content) {
        $('.loading-overdrive').css('display', 'none');
        if (content != '') {
          // Creating a span to manipulate the content
          var span = $('<span>');
          // Insert the content into the span
          span.html(content);

          if (content != 'No results were found.') {
            // Insert the content into overdrive fieldset
            overdrive.append(span.html());
            page_overdrive++;
            AjaxOverdrive(); // This line tells the function to run again once the initial AJAX call is complete.
          }
          else {
            //alert('Ended up stopping at page '+page_overdrive+' because there were no results found on that page.');
          }
        }
      }

      // Searching 3m
      var threeM     = $('#ebooks-3m');
      if (threeM.length > 0) {
         var search_keyword = $('#ebooks-last-search');
         var keyword_3m = search_keyword.attr('value');
         var page_3m = '2';
         if (only_available == 1) {
           url_3m = "/search/query/" + keyword_3m + "/source/loanable/page/" + page_3m + "/";
         }
         else {
           url_3m = "/search/query/" + keyword_3m + "/source/library/page/" + page_3m + "/";
         }
         if (threeM.html().replace('No results were found.', '').length == threeM.html().length) {
            Ajax3m();
         }
      }

      function Ajax3m() {
        $('.loading-3m').appendTo(threeM);
        $('.loading-3m').css('display', 'block');
        var searches = {
          url: url_3m
        };

        $.ajax({
          url:     ajaxUrlModel.replace("%s", "3m"),
          type:    'POST',
          success: threeMLoading,
          data:    searches
        });
      }

      function threeMLoading(content) {
        $('.loading-3m').css('display', 'none');
        if (content != '') {
          // Creating a span to manipulate the content
          var span = $('<span>');
          // Insert the content into the span
          span.html(content);

          //find for each book inside the span and get the first one
          var firstNew = $('.doc-link', span)[0];

          //find the link of the title inside the first item and get his text removing all spaces before and after
          var firstNewTitle = $('a[id*="title"]', firstNew).text().trim();

          //Getting all books previously searched
          var lastOlds = $('.doc-link', threeM);

          //Getting only the last book
          var lastOld = lastOlds[lastOlds.length-1];

          //Getting his title
          var lastOldTitle = $('a[id*="title"]', lastOld).text().trim();

          //Comparing if the last one and the new one has the same title
          if(firstNewTitle == lastOldTitle){
          //hide one of them
          lastOld.style.display = "none";
        }

        // Add the span with the modified content to the 3m panel
        threeM.append(span.html());

        page_3m++;
        if (only_available == 1) {
          url_3m = "/search/query/" + keyword_3m + "/source/loanable/page/" + page_3m + "/";
        }
        else {
          url_3m = "/search/query/" + keyword_3m + "/source/library/page/" + page_3m + "/";
        }
        Ajax3m();
        }
      }
    }
  };
})(jQuery);
