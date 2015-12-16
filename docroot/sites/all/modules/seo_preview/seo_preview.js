(function ($) {

  $(document).ready(function () {
    $("body").prepend('<div class="seo-preview"></div>');
    $("div.seo-preview").append('<a class="seo-preview-toggle seo-border-bottom" href="#">SEO Preview</a>');
    $("div.seo-preview").append('<div class="seo-title"></div>');
    $("div.seo-preview").append('<div class="seo-url"></div>');
    $("div.seo-preview").append('<div class="seo-description seo-border-bottom"></div>');
    $("div.seo-preview").append('<div class="seo-warnings"></div>');
    $("div.seo-preview").append('<div class="seo-tipps"></div>');

    if (Drupal.settings.seo_preview.minimize_by_default == 1) {
      $("div.seo-preview").addClass('seo-preview-hidden');
    }

    $(".seo-preview .seo-preview-toggle").click(function(event) {
      $(".seo-preview").toggleClass('seo-preview-hidden');
      event.preventDefault();
    });

    // Get page title
    var seo_title = $(document).attr('title');
    if (seo_title != undefined) {
      if (seo_title.length > 69) {
        seo_title = seo_title.substr(0, 69) + '...';
      }
    } else {
      seo_title = '';
    }
    $(".seo-preview .seo-title").html(seo_title);

    // Get URL
    var seo_pathname = window.location.href;
    seo_pathname = seo_pathname.replace('http://', "");
    seo_pathname = seo_pathname.split("#");
    seo_pathname = seo_pathname[0];
    $(".seo-preview .seo-url").html(seo_pathname);

    // Get meta description
    var seo_description = $("meta[name=description]").attr("content");
    if (seo_description != undefined) {
      var seo_description_visible = seo_description.substr(0, 156);
      var seo_description_invisible = '<span class="seo-description-fade">' + seo_description.substr(156, 1000) + '</span>';
      var seo_description_counter = '';
      if (seo_description) {
        seo_description_counter = ' <span class="seo-counter">(' + Drupal.t('Description @length of 156 characters', {'@length': seo_description.length}) + ')</span>';
      }
      $(".seo-preview .seo-description").html(seo_description_visible + seo_description_invisible + seo_description_counter);
    } else {
      seo_description = '';
    }

    // Display hints.
    if (Drupal.settings.seo_preview.display_hints == 1) {
      var seo_tipp_title = Drupal.t("<b>Title</b>: Your title Tag is <b>@length</b> characters long. Search engines use your page title to help determine where your website fits in the search results. Visitors will see and use your page title to help them determine if your site has what they're looking for and to remind them what your site is about when they see it in their bookmark list. A good page title achieves both objectives. Search engines will cut off a title that is too long - typically 65 characters. It's a good idea to place important keywords at the beginning of the title.", {'@length': seo_title.length});
      $("div.seo-preview .seo-tipps").append('<div class="seo-tipp seo-border-bottom">' + seo_tipp_title + '</div>');

      var seo_tipp_description = Drupal.t("<b>Description</b>: Your META-Description is <b>@length</b> characters long. A meta description consists of two or three sentences describing the content of the page. It is used by many search engines as the text under the link when your website is shown in the search results. It is not generally used for ranking purposes. The most important audience of your meta description is the human searcher. This is an opportunity to convince that person that your website is the best option for them to click on in the search results. Try to keep it below 160 characters so that it doesn't get cut o...", {'@length': seo_description.length});
      $("div.seo-preview .seo-tipps").append('<div class="seo-tipp seo-border-bottom">' + seo_tipp_description + '</div>');
    }

    // Warnings
    var warnings = 0;
    if (seo_description.length == 0) {
      $("div.seo-preview").addClass('seo-warning');
      $("div.seo-preview .seo-warnings").append('<div class="seo-border-bottom">' + Drupal.t('Meta description missing!') + '</div>');
      warnings = 1;
    }
    else if (seo_description.length < 50) {
      $("div.seo-preview").addClass('seo-warning');
      $("div.seo-preview .seo-warnings").append('<div class="seo-border-bottom">' + Drupal.t('Meta description too short!') + '</div>');
      warnings = 1;
    }
    else if (seo_description.length > 156) {
      $("div.seo-preview").addClass('seo-warning');
      $("div.seo-preview .seo-warnings").append('<div class="seo-border-bottom">' + Drupal.t('Meta description too long!') + '</div>');
      warnings = 1;
    }

    if (Drupal.settings.seo_preview.expand_on_warning == 1 && warnings == 1) {
      $("div.seo-preview").removeClass('seo-preview-hidden');
    }

  });

})(jQuery);
