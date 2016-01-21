(function ($) {
  Drupal.behaviors.epub = {
    attach: function (context) {

    },
    resizeIframe: function(target) {
      console.log('xxx');
      var $myIframe = $('#ebook iframe');
      var myIframe = $myIframe[0];
      var MutationObserver = window.MutationObserver || window.WebKitMutationObserver;

      myIframe.addEventListener('load', function() {
        var hash = myIframe.contentWindow.location.hash;
        setIframeHeight();
        var target = myIframe.contentDocument.body;
        var observer = new MutationObserver(function(mutations) {
          setIframeHeight();
        });
        var config = {
          attributes: true,
          childList: true,
          characterData: true,
          subtree: true
        };
        observer.observe(target, config);
      });

      function setIframeHeight() {
        $myIframe.height('auto');
        var newHeight = $('html', myIframe.contentDocument).height();
        $myIframe.height(newHeight);
        //$myIframe.attr('src', myIframe.contentWindow.location.href);
        console.log( myIframe.contentWindow.location.href);
      }
    },
    generateTocItems: function(contentsPath, toc, level, target) {
      var container = document.createElement("ul");
      if(!level) level = 1;
      toc.forEach(function(chapter) {
        var listitem = document.createElement("li"),
            link = document.createElement("a");
        var subitems;
        listitem.id = "toc-"+chapter.id;
        link.textContent = chapter.label;
        link.href = contentsPath+chapter.href;
        link.target = target;
        link.classList.add('toc_link');
        if(chapter.subitems) {
          level++;
          subitems = Drupal.behaviors.epub.generateTocItems(contentsPath, chapter.subitems, level, target);
          listitem.appendChild(subitems);
        }
        listitem.appendChild(link);
        container.appendChild(listitem);
      });
      return container;
    }
  };
})(jQuery);
