<?php drupal_add_css(drupal_get_path('module', 'chardin') . '/css/chardin.demo.css'); ?>

<div class="chardin-demo-page">
  <div class="jumbotron">
    <h1 data-intro="Project title" data-position="right">Chardin.js</h1>
    <p class="lead"> Simple overlay instructions for your apps. </p>
    <img class="chardin" src="/<?php print drupal_get_path('module', 'chardin') . '/images/chardin.png'; ?>"> <a href="#" id="chardin-toggle" class="btn btn-large primary">See it in action</a>
    <div class="credits">
      <p> Baked with <b>&lt;3</b> by <a href="https://github.com/heelhook">@heelhook</a>. </p>
    </div>
    <div class="links"> <a href="https://twitter.com/share" class="twitter-share-button" data-url="https://drupal.org/project/chardin" data-text="Check out Chardin.js module, simple overlay instructions for Drupal!" data-via="heelhookrc" data-count="none">Tweet</a> 
      <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+'://platform.twitter.com/widgets.js';fjs.parentNode.insertBefore(js,fjs);}}(document, 'script', 'twitter-wjs');</script> 
      <a href="https://twitter.com/heelhookrc" class="twitter-follow-button" data-show-count="false">Follow @heelhookrc</a> 
      <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+'://platform.twitter.com/widgets.js';fjs.parentNode.insertBefore(js,fjs);}}(document, 'script', 'twitter-wjs');</script> 
      <a class="hacker-news" href="https://news.ycombinator.com/item?id=5480073" style="margin-right: 2px;"> <i class="logo">Y</i> Hacker News </a> 
      <!--<div id="___plusone_0" style="text-indent: 0px; margin: 0px; padding: 0px; background-color: transparent; border-style: none; float: none; line-height: normal; font-size: 1px; vertical-align: baseline; display: inline-block; width: 32px; height: 20px; background-position: initial initial; background-repeat: initial initial;"> 
        <!-- Place this tag where you want the +1 button to render. -->
      <div class="g-plusone" data-size="medium" data-annotation="none" data-href="http://heelhook.github.io/chardin.js/"></div>
      
      <!-- Place this tag after the last +1 button tag. --> 
      <script type="text/javascript">
          (function() {
            var po = document.createElement('script'); po.type = 'text/javascript'; po.async = true;
            po.src = 'https://apis.google.com/js/plusone.js';
            var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(po, s);
          })();
        </script> 
      <!--</div> --> 
    </div>
    <div class="license"> Code licensed under the <a href="http://www.apache.org/licenses/LICENSE-2.0">Apache License v2.0</a>. </div>
  </div>
</div>
