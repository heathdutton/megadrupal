<?php
/**
 * @file
 * Backbone templates used by Getty Images.
 */

/**
 * Top-level modal container.
 */
?>
<script type="text/html" id="tmpl-getty-images-frame">
  <div class="getty-images-title"></div>
  <div class="getty-images-content"></div>
  <div class="getty-images-actionbar"></div>
</script>
<?php
/**
 * User panel.
 */
?>
<script type="text/html" id="tmpl-getty-images-user">
  <# var loginClass = '';

    if(data.loggingIn) {
      loginClass += 'getty-prompt-login getty-logging-in ';
    }
    else if(data.promptLogin) {
      loginClass += 'getty-prompt-login';
    } #>
  <div class="getty-user-panel {{ loginClass }}">
    <span class="getty-user-chevron"><span></span></span>
    <# if(!data.loggedIn) { #>
    <div class="getty-images-login">
      <p class="getty-login-username">
        <input type="text" class="getty-text-input" name="getty-login-username" value="{{ data.username }}" placeholder="<?php echo check_plain(t("Getty Images Login")); ?>" tabindex="20" />
        <a href="https://secure.gettyimages.com/forgot-username" class="forgot" target="_getty"><?php echo check_plain(t("Forgot username?")); ?></a>
      </p>
      <p class="getty-login-password">
        <input type="password" class="getty-text-input" name="getty-login-password" value="" placeholder="{{ data.loggingIn ? <?php echo json_encode(t("Logging in...")); ?> : <?php echo json_encode(t("Getty Images Password")); ?> }}" tabindex="30" />
        <a href="https://secure.gettyimages.com/Account/ForgotPassword.aspx" class="forgot" target="_getty"><?php echo check_plain(t("Forgot password?")); ?></a>
      </p>
      <p class="getty-login-submit">
        <input type="button" class="button-primary getty-login-button" value="<?php echo check_plain(t("Log in")); ?>" tabindex="3" />
        <span class="getty-login-spinner"></span>
      </p>
      <p class="no-login">
        <?php echo check_plain(t("Don't have a login?")); ?>
        <a href="https://secure.gettyimages.com/register/EnterUserInfo?AutoRedirect=true" target="_getty"><?php echo check_plain(t("Sign up to Getty Images")); ?></a>
      </p>

      <# if(data.error) { #>
      <div class="error-message">{{ data.error }}</div>
      <# } #>
    </div><#
    } else { #>
    <div class="logged-in-status">
      <p>
        <strong class="getty-images-username"><?php echo check_plain(t("Logged in as:")); ?> {{ data.username }}</strong>
      </p>
      <# if(data.products && data.products.length) { #>
      <p>
        <strong class="getty-images-product-offerings"><?php echo check_plain(t("Products:")); ?></strong>
        <# for(var i in data.products) { #>
          <span class="getty-images-product-offering">{{ data.products[i] }}</span>{{ i < data.products.length - 1 ? ', ' : '' }}
        <# } #>
      </p>
      <# } #>
      <p>
        <a href="#" class="getty-images-logout"><?php echo check_plain(t("log out")); ?></a>
      </p>
    </div>
    <# } #>
  </div>
</script>

<script type="text/html" id="tmpl-getty-title-bar">
  <h1 class="getty-images-title"><?php echo check_plain(t("Getty Images")); ?></h1>

  <div class="getty-title-links">
    <# var loggedIn = GettyImages.user.get('loggedIn'); #>
    <# if(data.mode == 'login' && loggedIn) { #>
    <span class="getty-title-link">
      <a class="getty-login-toggle getty-title-link {{ loggedIn ? 'getty-logged-in' : '' }}">{{ loggedIn ? GettyImages.user.get('username') : "<?php echo check_plain(t("Log in")); ?>" }}</a>
      <div class="getty-user-session"></div>
    </span>
    <# } #>
    <# if(data.mode == 'login' && !loggedIn) { #>
      <a class="getty-title-link getty-mode-change">Change Mode</a>
    <# } else if(data.mode == 'embed') { #>
      <a class="getty-title-link getty-mode-change">Go to Customer Login</a>
    <# } #>

    <a class="getty-title-link getty-about-link"><?php echo check_plain(t("About")); ?></a>
    <a class="getty-title-link getty-privacy-link" target="_getty" href="http://www.gettyimages.com/Corporate/PrivacyPolicy.aspx"><?php echo check_plain(t("Privacy Policy")); ?></a>
    <a class="getty-title-link getty-close-link">&times;</a>
  </div>
</script>

<script type="text/html" id="tmpl-getty-about-text">
  <a class="getty-about-close getty-about-close-x">X</a>

<?php
  /* TODO: Localize this text. */
  include __DIR__ . '/getty-about-en-us.html';
?>

  <p style="text-align: right">
    <a class="getty-about-close"><?php echo check_plain(t("Close")); ?></a>
  </p>
</script>

<script type="text/html" id="tmpl-getty-attachments-browser">
<div class="getty-browser-container">
  <table class="getty-browser-flex-container">
    <tbody>
      <tr>
        <td class="getty-search-toolbar"></td>
      </tr>
      <tr>
        <td class="getty-browser-body">
          <div class="getty-browser">
            <div class="getty-refine"></div>
            <div class="getty-results"></div>
            <div class="getty-search-spinner"></div>
          </div>
        </td>
      </tr>
    </tbody>
  </table>
</div>
<div class="getty-sidebar-container">
  <div class="getty-sidebar"></div>
</div>
</script>

<?php
/**
 * Selection of images.
 */
?>
<script type="text/html" id="tmpl-getty-images-selection">
  <div class="getty-selection-view"></div>
</script>

<script type="text/html" id="tmpl-getty-attachment">
<# var classes = [];
  classes.push('style-' + data.GraphicStyle);
  classes.push('licensing-' + data.LicensingModel);
  #>

  <div class="getty-attachment-preview {{ classes.join(' ') }}">
    <# if (data.uploading) { #>
      <div class="media-progress-bar"><div></div></div>
    <# } else { #>
      <div class="getty-thumbnail">
        <div class="getty-centered">
          <img src="{{ data.UrlThumb }}" draggable="false" />
        </div>
      </div>
      <div class="getty-image-synopsis">
        <span class="getty-image-id">#{{ data.ImageId }}</span>
        <span class="getty-image-collection">{{ data.CollectionName }}</span>
      </div>
    <# } #>

  </div>
</script>

<script type="text/html" id="tmpl-getty-download-authorizations">
<# if(GettyImages.user.settings.get('mode') != 'embed' && !GettyImages.user.get('loggedIn')) { #>
  <p><?php echo check_plain(t("Log in to download images")); ?></p>
<# }
else if(GettyImages.user.get('loggedIn') && data.DownloadAuthorizations) { #>
  <div class="getty-download-authorizations">
    <ul class="getty-download-with">
    <#
    var attachment = data.attachment;
    var authorizations = _.sortBy(data.DownloadAuthorizations, 'FileSizeInBytes');
    #>
    <li class="getty-download-auth">
      <label for="downloadSizeKey" class="header"><?php echo t("Download Options"); ?></label>
      <select name="DownloadSizeKey" id="downloadSizeKey">
      <# for(var i = 0; i < authorizations.length; i++) {
        var auth = authorizations[i], label = auth.ProductOfferingType, attrs = '', note = '';

        if(auth.DownloadIsFree) {
          note += ' (' + <?php echo json_encode(t("free")); ?> + ')';
        }
        else if(auth.DownloadsRemaining !== null) {
          note = ' (' + auth.DownloadsRemaining + ' ' + <?php echo json_encode(t("remaining")); ?> + ')';
        }

        // If we've got a downloaded version of this image, then
        // select it and mark it as the downloaded one. We only keep
        // track of one version of a downloaded image, so downloading a new
        // one will blow this away.
        if(attachment && attachment.get('width') == auth.PixelWidth && attachment.get('height') == auth.PixelHeight) {
          attrs = 'selected="selected" data-downloaded="true"';
          note = <?php echo json_encode(t("(downloaded)")); ?>;
        }

        if(data.DownloadSizeKey == auth.SizeKey) {
          attrs = 'selected="selected"';
        }

        var size = auth.FileSizeInBytes;

        if(size > 1024 * 1024) {
          size = new Number(Math.round(size / (1024 * 1024) * 10) / 10).commaString() + '\xA0MB';
        }
        else if(size > 1024) {
          size = new Number(Math.round(size / 1024 * 10) / 10).commaString() + '\xA0KB';
        }
        else {
          size = size + '\xA0B';
        }

        label += ': ' + auth.PixelWidth  + '×' + auth.PixelHeight + '  (' + size + ') ' + note;
        #>
        <option {{{ attrs }}} data-downloadtoken="{{ auth.DownloadToken }}" value="{{ auth.SizeKey }}">{{ label }}</option>
      <# } #>
      </select>
    </li>
    </ul>
  </div>

  <# if(data.DownloadSizeKey) { #>
  <div class="getty-download">
    <#
      var disabled = data.downloading ? 'disabled="disabled"' : '';
      var text = GettyImages.text.downloadImage;

      if(data.files && data.files.length) {
        text = GettyImages.text.reDownloadImage;
      }

      if(data.downloading) {
        text = GettyImages.text.downloading;
      }
    #>
    <input type="submit" class="getty-images-button button-primary" value="{{ text }}" {{ disabled }} />
    <div class="getty-download-spinner"></div>
  </div><#
  }
}
else if(data.authorizing) { #>
  <p class="description"><?php echo check_plain(t("Downloading authorizations...")); ?></p>
<# } #>
</script>

<script type="text/html" id="tmpl-getty-image-details-list">
<dl class="getty-image-details-list">
  <dt class="getty-title"><?php echo check_plain(t("Title:")); ?></dt>
  <dd class="getty-title">{{ data.Title }}</dd>

  <dt class="getty-image-id"><?php echo check_plain(t("Image #:")); ?></dt>
  <dd class="getty-image-id">{{ data.ImageId }}</dd>

  <dt class="getty-artist"><?php echo check_plain(t("Artist:")); ?></dt>
  <dd class="getty-artist">{{ data.Artist }}</dd>

  <dt class="getty-collection"><?php echo check_plain(t("Collection:")); ?></dt>
  <dd class="getty-collection">{{ data.CollectionName }}</dd>

  <# if(data.downloadingDetails) { #>
  <dt><?php echo check_plain(t("Downloading Details...")); ?></dt>
  <# } else if(!data.haveDetails) { #>
  <dt><?php echo check_plain(t("Could not not get image details.")); ?></dt>
  <# }#>

  <# if(data.ReleaseMessage) { #>
  <dt class="getty-release-info"><?php echo check_plain(t("Release Information:")); ?></dt>
  <dd class="getty-release-info"><p class="description">{{{ data.ReleaseMessage.gettyLinkifyText() }}}</p></dd>
  <# } #>

  <# if(data.Restrictions && data.Restrictions.length) { #>
    <dt class="getty-release-info"><?php echo check_plain(t("Restrictions:")); ?></dt>
    <# for(var i in data.Restrictions) { #>
    <dd class="getty-restrictions"><p class="description">{{{ data.Restrictions[i].gettyLinkifyText() }}}</p></dd>
    <# }
  }#>

  <dd class="getty-licensing">{{ data.Licensing }}</dd>
  <# if(data.Keywords) {
    var filter = function(kw) { return kw.Type == 'SpecificPeople'; };

    var people = _.filter(data.Keywords, filter);
    var keywords = _.reject(data.Keywords, filter);

    if(people.length) {
  #>
  <dt class="getty-keywords"><?php echo check_plain(t("People:")); ?></dt>
  <dd class="getty-keywords">
    <ul>
    <# for(var i = 0; i < people.length; i++) { #>
      <li class="getty-keyword"><a href="#keyword-{{ people[i].Id }}">{{ people[i].Text }}</a></li>
    <# } #>
    </ul>
  </dd>
  <# }
    if(keywords.length) {
  #>
  <dt class="getty-keywords"><?php echo check_plain(t("Keywords:")); ?></dt>
  <dd class="getty-keywords">
    <ul>
    <# for(var i = 0; i < keywords.length; i++) { #>
      <li class="getty-keyword"><a href="#keyword-{{ keywords[i].Id }}">{{ keywords[i].Text }}</a></li>
    <# } #>
    </ul>
  </dd>
  <# }
} #>
</dl>
</script>

<script type="text/html" id="tmpl-getty-detail-image">
<# if(data.ImageId) {
  var file = (data.files && data.files.length > 0) && data.files.at(0);
  var thumbUrl = file && file.get('url');

  if(!thumbUrl) {
    if(data.UrlWatermarkComp != "unavailable") {
      thumbUrl = data.UrlWatermarkComp;
    }
    else if(data.UrlComp != "unavailable") {
      thumbUrl = data.UrlComp;
    }
  }
  #>

  <div class="getty-thumbnail">
    <# if(thumbUrl) { #>
    <img src="{{ thumbUrl }}" class="getty-icon" draggable="false" />
    <# } else { #>
    <h3><?php echo check_plain(t("(Thumbnail Unavailable)")); ?></h3>
    <# } #>
  </div>

  <# if(file) { #>
    <div class="uploaded"><?php echo check_plain(t("Downloaded")); ?>: {{ file.get('downloaded') }}</div>
    <div class="dimensions">{{ file.get('width') }} &times; {{ file.get('height') }}</div>
  <# }
}  #>
</script>

<script type="text/html" id="tmpl-getty-image-details">
<# if(data.ImageId) {
  var attachment = data.attachment ? data.attachment.attributes : false; #>
  <h3>
    <?php echo check_plain(t("Image Details")); ?>
  </h3>

  <div class="attachment-info getty-attachment-details {{ data.downloading ? 'downloading' : '' }}">
    <div class="getty-image-thumbnail"></div>
    <div class="getty-image-details-list"></div>
    <div class="getty-image-sizes"></div>
    <div class="getty-download-authorizations"></div>
  </div>
<# } #>
</script>

<script type="text/html" id="tmpl-getty-display-settings">
<# if(GettyImages.user.get('loggedIn')) { #>
  <h3><?php echo check_plain(t("Display Options")); ?></h3>

  <p class="getty-display-setting">
    <label><?php echo check_plain(t("Align")); ?></label>
    <select data-setting="align" data-user-setting="getty_align">
      <# selected = data.model.align == 'none' ? 'selected="selected"' : '' #>
      <option value="none" {{ selected }}>
        <?php echo check_plain(t("None")); ?>
      </option>
      <# selected = data.model.align == 'left' ? 'selected="selected"' : '' #>
      <option value="left" {{ selected }}>
        <?php echo check_plain(t("Left")); ?>
      </option>
      <# selected = data.model.align == 'center' ? 'selected="selected"' : '' #>
      <option value="center" {{ selected }}>
        <?php echo check_plain(t("Center")); ?>
      </option>
      <# selected = data.model.align == 'right' ? 'selected="selected"' : '' #>
      <option value="right" {{ selected }}>
        <?php echo check_plain(t("Right")); ?>
      </option>
    </select>
  </p>

  <p class="getty-display-setting">
    <label><?php echo check_plain(t("Size")); ?></label>
    <select class="size" name="size" data-setting="size" data-user-setting="getty_imgsize">
    <# _.each(data.model.get('sizes'), function(size, value) {
      var selected = data.model.get('size') == size ? 'selected="selected"' : '';
      #>
      <option value="{{ value }}" {{{ selected }}}>{{ size.label }} &ndash; {{ parseInt(size.width) }} &times; {{ parseInt(size.height) }}</option>
    <# }); #>
    </select>
  </label>

  <p class="getty-display-setting">
    <label><?php echo check_plain(t("Alt Text")); ?></label>
    <input type="text" data-setting="alt" value="{{ data.model.get('alt') }}" data-user-setting="getty_alt" />
  </p>

  <p class="getty-display-setting">
    <label><?php echo check_plain(t("Caption")); ?></label>
    <textarea data-setting="caption">{{ data.model.get('caption') }}</textarea>
  </p>
<# } else { #>
<dl class="getty-image-details-list">
  <dt class="getty-image-caption"><?php echo check_plain(t("Caption:")); ?></dt>
  <dd class="getty-image-caption"><p class="description">{{ data.model.get('caption') }}</p></dd>

  <dt class="getty-image-alt"><?php echo check_plain(t("Alt Text:")); ?></dt>
  <dd class="getty-image-alt"><p class="description">{{ data.model.get('alt') }}</p></dd>
</dl>
<# } #>
</script>

<script type="text/html" id="tmpl-getty-result-refinement-category">
  <div class="getty-refinement-category-name">
    {{ data.id.reverseCamelGirl() }}
    <span class="getty-refinement-category-arrow"></span>
  </div>
  <ul class="getty-refinement-list"></ul>
</script>

<script type="text/html" id="tmpl-getty-result-refinement-option">
  <# if(!data.active) { #>
  <a href="#" title="{{ data.text }}">{{ data.text }} <span class="count">{{ new Number(data.count).commaString() }}</span></a>
  <# } #>
</script>

<script type="text/html" id="tmpl-getty-result-refinement">
  <span class="getty-remove-refinement">X</span>
  <# if(data.category) { #>
    <strong>{{ data.category.reverseCamelGirl() }}</strong>: <span>{{ data.text }}</span>
  <# } else { #>
    <em>{{ data.text }}</em>
  <# } #>
</script>

<script type="text/html" id="tmpl-getty-images-more">
  <div class="getty-attachment-preview">
    <div class="getty-more-spinner">
    </div>
    <div class="getty-more-text-container">
      <span class="getty-number-remaining"></span>
      <span class="getty-more-text"><?php echo check_plain(t("More")); ?></span>
    </div>
  </div>
</script>

<script type="text/html" id="tmpl-getty-comp-license-agreement">
  <p class="getty-comp-please-read"><?php echo check_plain(t("Please read and accept the following terms before using image comps in your website:")); ?></p>
  <div class="getty-comp-license-frame"><?php
    /* TODO: Localize this text. */
    include __DIR__ . '/getty-comp-license.html';
  ?></div>
  <div class="getty-comp-buttons">
  <input type="button" class="button-primary" value="<?php echo check_plain(t("Agree")); ?>" />
    &nbsp;
    &nbsp;
    &nbsp;
    <a href="javascript:void(0);" class="getty-cancel-link">Cancel</a>
  </div>
  <div class="getty-comp-license-chevron"></div>
</script>

<script type="text/html" id="tmpl-getty-unsupported-browser">
  <h1><?php echo check_plain(t("Sorry, this browser is unsupported!")); ?></h1>

  <p><?php echo check_plain(t("The Getty Images plugin requires at least Internet Explorer 10 to function. This plugin also supports other modern browsers with proper CORS support such as Firefox, Chrome, Safari, Opera.")); ?></p>
</script>

<script type="text/html" id="tmpl-getty-choose-mode">
  <# if(data.mode != 'login') { #>
  <div class="getty-split-panel getty-embedded-mode">
    <div class="getty-panel">
      <div class="getty-panel-content">
      <h1><?php echo check_plain(t("Access Embeddable Images")); ?></h1>

        <p><?php echo t("Choose from over <strong>50 million</strong> high-quality hosted images, available for free, non-commercial use in your Drupal site."); ?></p>
      </div>
      <span class="getty-icon icon-image"></span>
    </div>
  </div>

  <div class="getty-split-panel getty-login-mode">
  <# } #>
    <div class="getty-panel">
      <div class="getty-panel-content">
        <div class="getty-login-mode-panel">
          <h1><?php echo check_plain(t("Getty Images Customer")); ?></h1>
          <p><?php echo check_plain(t("Log into your Getty Images account to access all content and usage rights available in your subscription.")); ?></p>
        </div>

        <div class="getty-login-panel">
        </div>
      </div>
      <# if(data.mode != 'login') { #>
        <span class="getty-icon icon-unlocked"></span>
      <# } #>
    </div>

  <# if(data.mode != 'login') { #>
  </div>
  <# } #>
  </div>
</script>
<?php
/**
 * CSS template for resizing attachments.
 */
?>
<script type="text/html" id="tmpl-getty-attachments-css">
  <style type="text/css" id="{{ data.id }}-css">
    #{{ data.id }} {
      padding: 0 {{ data.gutter }}px;
    }

    #{{ data.id }} .getty-attachment {
      margin: {{ data.gutter }}px;
      width: {{ data.edge }}px;
    }

    #{{ data.id }} .getty-attachment-preview,
    #{{ data.id }} .getty-attachment-preview .getty-thumbnail {
      width: {{ data.edge }}px;
      height: {{ data.edge }}px;
    }
  </style>
</script>
<?php
/**
 * The "clipboard" view.
 */
?>
<script type="text/html" id="tmpl-getty-clipboard">
  <a href="javascript:void(0)" title="{{ data.label }}" class="icon-clipboard"></a>
</script>
