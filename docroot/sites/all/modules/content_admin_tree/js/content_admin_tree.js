(function ($) {
  /**
   * This serves several purposes to do with page output.
   * Within the PHP of the module it is easy to alternate between
   * leaf and expanded classes for each node type.
   * But the taxonomy term tree is made into a html block
   * through the use of custom functions because of the complexity
   * of rendering a taxonomy tree. In this case the only way to alter
   * the state of ULs and LIs within the taxonomy tree we can use
   * Javascript and JQuery to get the current url and set the term for
   * that url as active.
   * This also deals with dynamically changing the width of the content
   * view based on the page width and the width of the menu tree.
   */

  Drupal.behaviors.contentAdminTree = {
    attach: function (context, settings) {
      /**
       * A function to resize the views block.
       */
      function viewsBlockResize() {
        // Get current page width.
        var pageWidth = $('#page').width();
        // Get current menu tree width.
        var menuWidth = $('.cat-menu').width();
        // Make the width of the view block
        // the width of the page - the width of the menu tree.
        var blockWidth = pageWidth - menuWidth - 70;
        // Set the width of the views block to blockWidth.
        $('.cat-views-block').css({ 'width' : blockWidth});
      }
      // Call the function.
      viewsBlockResize();
      // If window has been resized change the size
      // of the views block.
      $(window).resize(function() {
        viewsBlockResize();
      });
      /**
       * This next section deals with setting classes
       * of taxonomy terms based on the current url.
       */
      var currentPath = window.location.href;
      // Where the page is an admin overlay ignore the fact
      // that it is an overlay by removing overlay from the url.
      if (currentPath.indexOf("?render=overlay") !== -1) {
        $('div.cat-menu').addClass('cat-overlay');
        $('div.cat-views-block').addClass('cat-overlay');
        currentPath = currentPath.replace("?render=overlay", "");
      }
      // Get the path alias for node-type/tax-term.
      var adminPathAlias = currentPath.split("/admin/content/")[1];
      // Where forward slash exists create an array
      // split by the forward slash.
      if (adminPathAlias != null) {
        if (adminPathAlias.indexOf("/") !== -1) {
          var arrayAPA = adminPathAlias.split("/");
          var length = arrayAPA.length;
          // Then check if the length of the array is 3
          // wildcards being node-type/overview/tax-term.
          if (length === 3) {
            // replace dashes from the url taxonomy
            // term and node type with spaces.
            var taxTermID = arrayAPA[2].replace(/-/g, " ");
            var nodeType = arrayAPA[0].replace(/-/g, " ");
            // loop through each list item with the leaf class.
            $('li.leaf').each(function() {
              // Get the link's URL.
              var taxonomyTermURL = $(this).find('> a').attr('href');
              // Explode the link into an array seperate by /.
              var taxonomyTermURLArray = taxonomyTermURL.split("/");
              // Get the position of the Term ID.
              var termIDPos = taxonomyTermURLArray.length - 1;
              // Get the term ID text directly from the cat-link url.
              anchorTaxonomyTermID = taxonomyTermURLArray[termIDPos];
              // Get the parent node type text.
              var parentNodeTypeText = $(this).closest('.cat-node-type').find('> a').text().toLowerCase();
              // Where the taxonomy term from the url
              // is identical to the term from the anchor text
              // and the parent node type is the same as in the URL.
              if (anchorTaxonomyTermID === taxTermID && nodeType === parentNodeTypeText) {
                // remove the leaf class and add the expanded
                // and active classes.
                $(this).removeClass('leaf');
                $(this).addClass('expanded active');
                // Where the parent List item is leaf
                // remove the leaf class and add expanded class.
                if ($(this).parent().parent().hasClass('leaf')) {
                  // Use boolean to loop through and check
                  // if the parent has parents, remove the old
                  // class and assign the new classes if true.
                  var hasLIParents = true;
                  var liParent = $(this).parent().parent();
                  while(hasLIParents === true) {
                    liParent.removeClass('leaf');
                    liParent.addClass('expanded active-trail');
                    if ($(liParent).parent().parent().hasClass('leaf')) {
                      liParent = $(liParent).parent().parent();
                    }
                    else {
                      hasLIParents = false;
                    }
                  }
                }
                // Change the size of views block depending on the new size of the menu.
                viewsBlockResize();
              }
            });
          }
        }
      }
    }
  };
}) (jQuery);
