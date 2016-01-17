<?php
/**
 * Override of the Search Box for d6
 * first, select the form ID
 */
function jq_theme_theme() {
    return array(
    // The form ID.
    'search_block_form' => array(
      // Forms always take the form argument.
      'arguments' => array('form' => NULL),
    ),
  );
}
/**
  * Theme override for search form.
  * Paste this in the template.php file of your theme
  * more infos : lullabot.com/articles/modifying-forms-5-and-6
  * Here is where you can modify the selected form
  */

function jq_theme_form_alter(&$form, &$form_state, $form_id) {
	 if ($form_id == 'search_block_form') {
      $form['actions']['submit']['#value'] = 'GO'; // Change the text on the submit button
      $form['search_block_form']['#attributes']['placeholder'] =  t('Search');
  }
	}
/**
 * preprocess hook to add variables to the page.tpl.php
 */
function jq_theme_preprocess_page(&$variables) {
    global $base_path;
    $out ='';
    $sql="SELECT node.nid AS nid,
            node.title AS node_title,
            field_revision_body.body_value AS node_revisions_body,
            node.created AS node_created
            FROM node 
            LEFT JOIN field_revision_body ON node.vid = field_revision_body.revision_id 
            where node.status=1 and node.promote=1
            ORDER BY node_created DESC
            LIMIT 0,7
            
            ";
      
    $res = db_query($sql);
    //print_r($res);
    $details = $res->fetchAll();
    //print_r($details);
    
    $sql_count="SELECT count(*) FROM {node} where node.status =1";
    /* already if (db_result(db_query($sql_count))) {
        $out='<span id="leftControl" class="control" style="display: block;">
                        Clicking moves left
                    </span>';
    } commented*/ 
    $out .='<div id="slidesContainer" style="overflow: hidden;">';
    
    
    // print $count ;
    $count = db_query($sql_count);
    //print_r($count);
    $counts = $count->fetchAll();
    if ($counts != 0) {
    	  $out .='<div id="slideInner" style="width: 3360px; margin-left: 0px;">';
        $result = db_query($sql);
        $rowval = $result->fetchAll();
       // while ($rowval = db_fetch_object($result)) {
       	foreach ($rowval as $res){
            $out .='<div class="slide" style="float: left; width: 480px;">';
            $out .='<h1 class="slide_header">
                        <a title="Wish list for a new theme" rel="bookmark" href="'. $base_path .'node/'. $res->nid .'">'.
                            $res->node_title .'
                        </a>
                    </h1>';
            $out .='<div class="clearfix"></div>';
            $out .='<p class="post_info_slide">'. drupal_substr($res->node_revisions_body, 0, 25) .'</p>';
            $out .='</div>';
        }
        $out .='</div>';
    }
    else {
        $out .='<div id="slideInner" style="width: 480px;">';
        $out .='<div class="slide" style="float: left; width: 480px;">';
        $out .='<h1 class="slide_header">
                            <a title="zyxware" rel="bookmark" href="http://www.zyxware.com">
                                zyxware
                            </a>
                        </h1>';
        $out .='<div class="clearfix"></div>';
        $out .='<p class="post_info_slide">zyxware</p>';
        $out .='</div></div>';
    }
    $out .='</div>';
    $variables['slide_show']=$out;
    //print_r($out);
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
    $out='<ul>';
    if (!empty($breadcrumb)) {
        //print_r ($breadcrumb);
        foreach ($breadcrumb as $key => $val) {
            $out .='<li>'. $val .'</li>';
        }
      }
      else {
        global $base_path;
        $out .='<li><a href="'. $base_path .'">Home</a></li>';
    }
    $out .='</ul>';
    return $out;
}

function jq_theme_preprocess_node(&$variables) {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nid = arg(1);
    $node = node_load($nid);
    if($node->type == 'article') {
      $variables['article_date'] = 1;
    }
  }
}
