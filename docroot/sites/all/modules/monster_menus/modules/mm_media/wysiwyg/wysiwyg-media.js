/**
 *  @file
 *  Attach MM Media WYSIWYG behaviors.
 */

(function ($) {

Drupal.mm_media = Drupal.mm_media || {};

// Define the behavior.
Drupal.wysiwyg.plugins.mm_media = {
  childWindow: false,

  /**
   * Initializes the tag map.
   */
  initializeTagMap: function () {
    // This uses the tagmap defined in media/includes/media.filter.inc.
    if (typeof Drupal.settings.tagmap == 'undefined') {
      Drupal.settings.tagmap = { };
    }
  },
  /**
   * Execute the button.
   */
  invoke: function (data, settings, instanceId) {
    if (data.format == 'html') {
      this.childWindow = window.open(Drupal.settings.mm_media.browserUrl + '&instanceId=' + instanceId, 'mmMediaChild', 'directories=no,height=450,width=900,location=no,menubar=no,status=no,toolbar=no');
      if (!this.boundUnload) {
        this.boundUnload = true;
        $(window).unload(function() {
          Drupal.wysiwyg.plugins.mm_media.closeChildWindow();
        });
      }
    }
  },

  closeChildWindow: function () {
    if (this.childWindow) {
      this.childWindow.close();
      this.childWindow = false;
    }
  },

  insertMediaFile: function (mediaFile, viewMode, formattedMedia, options, wysiwygInstance) {

    this.initializeTagMap();
    // @TODO: the folks @ ckeditor have told us that there is no way
    // to reliably add wrapper divs via normal HTML.
    // There is some method of adding a "fake element"
    // But until then, we're just going to embed to img.
    // This is pretty hacked for now.
    //
    var imgElement = $(this.stripDivs(formattedMedia));
    this.addImageAttributes(imgElement, mediaFile.fid, viewMode, options);

    var toInsert = this.outerHTML(imgElement);
    // Create an inline tag
    var inlineTag = Drupal.wysiwyg.plugins.mm_media.createTag(imgElement);
    // Add it to the tag map in case the user switches input formats
    Drupal.settings.tagmap[inlineTag] = toInsert;
    wysiwygInstance.insert(toInsert);
  },

  /**
   * Gets the HTML content of an element
   *
   * @param jQuery element
   */
  outerHTML: function (element) {
    return $('<div>').append(element.eq(0).clone()).html();
  },

  addImageAttributes: function (imgElement, fid, view_mode, additional) {
    //    imgElement.attr('fid', fid);
    //    imgElement.attr('view_mode', view_mode);
    // Class so we can find this image later.
    imgElement.addClass('media-image');
    this.forceAttributesIntoClass(imgElement, fid, view_mode, additional);
    if (additional) {
      for (var k in additional) {
        if (additional.hasOwnProperty(k)) {
          if (k === 'attr') {
            imgElement.attr(k, additional[k]);
          }
        }
      }
    }
  },

  /**
   * Due to problems handling wrapping divs in ckeditor, this is needed.
   *
   * Going forward, if we don't care about supporting other editors
   * we can use the fakeobjects plugin to ckeditor to provide cleaner
   * transparency between what Drupal will output <div class="field..."><img></div>
   * instead of just <img>, for now though, we're going to remove all the stuff surrounding the images.
   *
   * @param String formattedMedia
   *  Element containing the image
   *
   * @return HTML of <img> tag inside formattedMedia
   */
  stripDivs: function (formattedMedia) {
    // Isolate just the image tag
    if ($(formattedMedia).is('img')) {
      return this.outerHTML($(formattedMedia));
    }
    return this.outerHTML($('img', $(formattedMedia)));
  },

  /**
   * Attach function, called when a rich text editor loads.
   * This finds all [[tags]] and replaces them with the html
   * that needs to show in the editor.
   */
  attach: function (content) {
    var matches = content.match(/\[\[.*?\]\]/g);
    this.initializeTagMap();
    var tagmap = Drupal.settings.tagmap;
    if (matches) {
      var inlineTag = "";
      for (var i = 0; i < matches.length; i++) {
        inlineTag = matches[i];
        if (tagmap[inlineTag]) {
          // This probably needs some work...
          // We need to somehow get the fid propogated here.
          // We really want to
          var tagContent = tagmap[inlineTag];
          var mediaMarkup = this.stripDivs(tagContent); // THis is <div>..<img>

          var _tag = inlineTag, mediaObj;
          _tag = _tag.replace('[[','');
          _tag = _tag.replace(']]','');
          try {
            mediaObj = JSON.parse(_tag);
          }
          catch (err) {
            mediaObj = null;
          }
          if (mediaObj) {
            var imgElement = $(mediaMarkup);
            this.addImageAttributes(imgElement, mediaObj.fid, mediaObj.view_mode, mediaObj.attributes);
            var toInsert = this.outerHTML(imgElement);
            content = content.replace(inlineTag, toInsert);
          }
        }
        else {
          debug.debug("Could not find content for " + inlineTag);
        }
      }
    }
    matches = content.match(/\[video\:.*?\]/g);
    if (matches) {
      var inlineTag = "";
      for (i = 0; i < matches.length; i++) {
        inlineTag = matches[i];
        if (tagmap[inlineTag]) {
          var tagContent = tagmap[inlineTag];
          if (tagContent.indexOf('image-x-generic.png')) {
            var temp_src = inlineTag.match(/src\:(.*?);/g);
            if (temp_src) {
              temp_src = temp_src[0];
              temp_src = temp_src.replace(':', '="').replace(';', '"');
              tagContent = tagContent.replace(/src=".*?"/g, temp_src).replace(/height=".*?px"/g, '').replace(/width=".*?px"/g, '');
              console.log(tagContent);
            }
          }
          var mediaMarkup = this.stripDivs(tagContent); // THis is <div>..<img>
          var _tag = inlineTag, mediaObj;
          mediaObj = this.setMediaObjectForVideo(_tag);
          if (mediaObj) {
            var imgElement = $(mediaMarkup);
            this.addImageAttributes(imgElement, mediaObj.fid, mediaObj.view_mode, mediaObj.attributes);
            var toInsert = this.outerHTML(imgElement);
            content = content.replace(inlineTag, toInsert);
          }
        }
        else {
          debug.debug("Could not find content for " + inlineTag);
        }
      }
    }
    return content;
  },

  /**
   * Detach function, called when a rich text editor detaches
   */
  detach: function (content) {
    // Replace all Media placeholder images with the appropriate inline json
    // string. Using a regular expression instead of jQuery manipulation to
    // prevent <script> tags from being displaced.
    // @see http://drupal.org/node/1280758.
    var matches;
    if (matches = content.match(/<img[^>]+class=([\'"])[a-z\-\s\+]*media-image[^>]*>/gi)) {
      for (var i = 0; i < matches.length; i++) {
        var imageTag = matches[i];
        var inlineTag = Drupal.wysiwyg.plugins.mm_media.createTag($(imageTag));
        Drupal.settings.tagmap[inlineTag] = imageTag;
        content = content.replace(imageTag, inlineTag);
      }
    }
    this.closeChildWindow();
    return content;
  },

  /**
   * @param jQuery imgNode
   *  Image node to create tag from
   */
  createTag: function (imgNode) {
    // Currently this is the <img> itself
    // Collect all attribs to be stashed into tagContent
    var mediaAttributes = {};
    var imgElement = imgNode[0];
    var sorter = [];
    var isVideo = false;
    // @todo: this does not work in IE, width and height are always 0.
    for (var i = 0; i < imgElement.attributes.length; i++) {
      var attr = imgElement.attributes[i];
      if (attr.specified == true) {
        sorter.push(attr);
      }
    }

    sorter.sort(this.sortAttributes);

    for (var prop in sorter) {
      mediaAttributes[sorter[prop].name.replace(/^data-/, '')] = sorter[prop].value;
      if (sorter[prop].name == 'data-video') {
        isVideo = true;
      }
    }

    // The following 5 ifs are dedicated to IE7
    // If the style is null, it is because IE7 can't read values from itself
    if (jQuery.browser.msie && jQuery.browser.version == '7.0') {
      if (mediaAttributes.style === "null") {
        var imgHeight = imgNode.css('height');
        var imgWidth = imgNode.css('width');
        mediaAttributes.style = {
          height: imgHeight,
          width: imgWidth
        };
        if (!mediaAttributes['width']) {
          mediaAttributes['width'] = imgWidth;
        }
        if (!mediaAttributes['height']) {
          mediaAttributes['height'] = imgHeight;
        }
      }
      // If the attribute width is zero, get the CSS width
      if (Number(mediaAttributes['width']) === 0) {
        mediaAttributes['width'] = imgNode.css('width');
      }
      // IE7 does support 'auto' as a value of the width attribute. It will not
      // display the image if this value is allowed to pass through
      if (mediaAttributes['width'] === 'auto') {
        delete mediaAttributes['width'];
      }
      // If the attribute height is zero, get the CSS height
      if (Number(mediaAttributes['height']) === 0) {
        mediaAttributes['height'] = imgNode.css('height');
      }
      // IE7 does support 'auto' as a value of the height attribute. It will not
      // display the image if this value is allowed to pass through
      if (mediaAttributes['height'] === 'auto') {
        delete mediaAttributes['height'];
      }
    }

    // Remove elements from attribs using the blacklist
    var view_mode = mediaAttributes.view_mode;
    var fid = mediaAttributes.fid;
    if (isVideo) {
      var video = mediaAttributes.video;
      var additional = '';
      delete mediaAttributes['video'];
        for (var property in mediaAttributes) {
          additional += '; ' + property + ':' + mediaAttributes[property];
        }
      return '[video:' + video + additional + '; ]';
    }
    else {
      for (var blackList in Drupal.settings.mm_media.blacklist) {
        delete mediaAttributes[Drupal.settings.mm_media.blacklist[blackList]];
      }
      var tagContent = {
        "type": 'media',
        "view_mode": view_mode,
        "fid" : fid,
        "attributes": mediaAttributes
      };
      return '[[' + JSON.stringify(tagContent) + ']]';
    }
  },

  /**
   * Forces custom attributes into the class field of the specified image.
   *
   * Due to a bug in some versions of Firefox
   * (http://forums.mozillazine.org/viewtopic.php?f=9&t=1991855), the
   * custom attributes used to share information about the image are
   * being stripped as the image markup is set into the rich text
   * editor.  Here we encode these attributes into the class field so
   * the data survives.
   *
   * @param imgElement
   *   The image
   * @fid
   *   The file id.
   * @param view_mode
   *   The view mode.
   * @param additional
   *   Additional attributes to add to the image.
   */
  forceAttributesIntoClass: function (imgElement, fid, view_mode, additional) {
    var wysiwyg = imgElement.attr('wysiwyg');
    if (wysiwyg) {
      imgElement.attr('data-wysiwyg', wysiwyg);
    }
    var format = imgElement.attr('format');
    if (format) {
      imgElement.attr('data-format', format);
    }
    var typeOf = imgElement.attr('typeof');
    if (typeOf) {
      imgElement.attr('data-typeof', typeOf);
    }
    if (fid) {
      imgElement.attr('data-fid', fid);
    }
    if (view_mode) {
      imgElement.attr('data-view_mode', view_mode);
    }
    if (additional) {
      for (var name in additional) {
        if (additional.hasOwnProperty(name)) {
          if (name !== 'alt') {
            imgElement.attr('data-' + name, additional[name]);
          }
        }
      }
    }
  },

  /*
   *
   */
  sortAttributes: function (a, b) {
    var nameA = a.name.toLowerCase();
    var nameB = b.name.toLowerCase();
    if (nameA < nameB) {
      return -1;
    }
    if (nameA > nameB) {
      return 1;
    }
    return 0;
  },

  setMediaObjectForVideo: function (tagString) {
    tagString = tagString.replace(/^\[/, "").replace(/\]$/, "");
    var tagArray = tagString.split('; ');
    if (tagArray.length > 0) {
      var mediaObj = {};
      mediaObj.attributes = {};
      var subTag = {};
      for (var k = 0; k < tagArray.length; k++) {
        subTag = tagArray[k].split(':');
        if (subTag.length > 2) {
          for (var j = 2; j < subTag.length; j++) {
            subTag[1] += ':' + subTag[j];
          }
        }
        if (subTag[0] == 'fid' || subTag[0] == 'view_mode') {
          mediaObj[subTag[0]] = subTag[1];
        }
        else {
          if (mediaObj.attributes == undefined) {
            mediaObj.attributes = {};
          }
          mediaObj.attributes[subTag[0]] = subTag[1];
        }
      }
      return mediaObj;
    }
    else {
      return false;
    }
  }
};

})(jQuery);
