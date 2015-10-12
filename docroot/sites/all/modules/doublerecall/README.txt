--- Installation ---

1. Enable module the standard Drupal way (admin/modules/list).
2. Configure DoubleRecall-related settings (admin/config/services/doublerecall).
3. Position DoubleRecall placeholder block on your site. This module holds container
   for DoubleRecall widget.
4. Enable node types, that should be hidden by DoubleRecall. This will hide entire
   node content, leaving only title. This should work for most of the people. 
   
   If you need more control add class="dr_article" and style="display:none" to 
   content elements that should be revealed by DoubleRecall. You will most likely 
   have to implement hook_doublerecall_should_hide() or hook_doublerecall_should_hide_alter().

   So if your content looks something like so:

  <div>
    <p>Lorem Ipsum dolor sit amet ...</p>
    <p> ... </p>
  </div>

  It should end up looking like this:

  <div class="dr_article" style="display:none">
    <p>Lorem Ipsum dolor sit amet ...</p>
    <p> ... </p>
  </div>

  
--- Author ---
Janez Urevc (slashrsm), http://drupal.org/user/744628