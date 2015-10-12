<?php

/* Common Drupal methods definitons using in Artisteer theme export */

/**
 * Generate the HTML representing a given menu with Artisteer style.
 *
*/
function art_menu_worker($content = NULL, $show_sub_menus, $menu_class) {
  if (!$content) {
    return '';
  }

  $output = $content;
  // used to support Menutrails module
  $output = str_replace("active-trail", "active-trail active", $output);
  
  $empty_str = '';
  $menu_str = ' class="menu"';
  if(strpos($output, $menu_str) !== FALSE) {
    $pattern = '/class="menu"/i';
    $replacement = 'class="'. $menu_class .'"';
    $output = preg_replace($pattern, $replacement, $output, 1);
    $output = str_replace($menu_str, $empty_str, $output);
  }

  if (class_exists('DOMDocument')) {
    $output = art_menu_xml_parcer($output, $show_sub_menus, $menu_class);
    /* Support Block Edit Link module */
	  $output = str_replace('<!DOCTYPE root>', $empty_str, $output);
  }
  else {
    $output = preg_replace('~(<a [^>]*>)([^<]*)(</a>)~', '$1<span class="l"></span><span class="r"></span><span class="t">$2</span>$3', $output);
  }
  
  return $output;
}

function art_menu_xml_parcer($content, $show_sub_menus, $menu_class) {
  $parent_id = $menu_class . '-id';

  $doc = art_xml_document_creator($content, $parent_id);
  if ($doc === FALSE) {
	  return $content; // An error occurred while reading XML content
  }

  $parent = $doc->documentElement;
  $elements = $parent->childNodes;
  $ul_elements = $doc->getElementsByTagName("ul");
  $ul_element = NULL;
  foreach($ul_elements as $ul_element) {
		// First ul element with css-class b2-hmenu or b2-vmenu
    if (($ul_element->getAttribute('class') == "b2-vmenu") || ($ul_element->getAttribute('class') == "b2-hmenu"))
		  break;
    continue;
  }

  if ($ul_element == NULL) return $content;
  $ul_children = art_menu_style_parcer($doc, $ul_element->childNodes, $show_sub_menus);

  $parent->appendChild($ul_element);
  while ($ul_element->previousSibling)
    $parent->removeChild($ul_element->previousSibling);

  return html_entity_decode($doc->saveHTML(), ENT_NOQUOTES, "UTF-8");
}

function art_xml_document_creator($content, $parent_id) {
  $old_error_handler = set_error_handler('art_handle_xml_error');
  $dom = new DOMDocument();
  /* Support Block Edit Link module */
  $doc_content = <<< XML
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE root [
<!ENTITY nbsp "&#160;">
]>
<div id="$parent_id">$content</div>
XML;
 
  $dom->loadXml($doc_content);   
  restore_error_handler();
  return $dom;
}

function art_handle_xml_error($errno, $errstr, $errfile, $errline) {
  if ($errno==E_WARNING && (substr_count($errstr,"DOMDocument::loadXML()")>0))
   return false; // An error occurred while reading XML content
  else 
    return true; // Successful
}

function art_menu_style_parcer($doc, $elements, $show_sub_menus) {
  $parentNodes_to_delete = array();
  $childNodes_to_delete = array();
  foreach ($elements as $element) {
    if (is_a($element, "DOMElement") && ($element->tagName == "li")) {
      $children = $element->childNodes;
      $parent_class = $element->getAttribute("class");
      $is_parent_class_active = strpos($parent_class, "active") !== FALSE;

      foreach ($children as $child) {
        if (is_a($child, "DOMElement") && ($child->tagName == "a")) {
          $caption = $child->nodeValue;
          if (empty($caption) || $caption=='test') {
            $childNodes_to_delete[] = $child;
            $parentNodes_to_delete[] = $element;
            break;
          }

          $child->nodeValue = "";
          if ($is_parent_class_active) {
            $child->setAttribute("class", $child->getAttribute("class").' active');
          }

          $spanL = $doc->createElement("span");
          $spanL->setAttribute("class", "l");
          //$spanL->nodeValue = "&nbsp;";
          $child->appendChild($spanL);

          $spanR = $doc->createElement("span");
          $spanR->setAttribute("class", "r");
          //$spanR->nodeValue = "&nbsp;";
          $child->appendChild($spanR);

          $spanT = $doc->createElement("span");
          $spanT->setAttribute("class", "t");
          $spanT->nodeValue = check_plain($caption);
          $child->appendChild($spanT);
        }
        else if (!$show_sub_menus) {
          $childNodes_to_delete[] = $child;
        }
      }
    }
  }

  art_remove_elements($childNodes_to_delete);
  art_remove_elements($parentNodes_to_delete);
  return $elements;
}

function art_remove_elements($elements_to_delete) {
  if (!isset($elements_to_delete)) return;
  foreach($elements_to_delete as $element) {
    if ($element != null) {
      $element->parentNode->removeChild($element);
    }
  }
}

function art_node_worker($node) {
  $links_output = art_links_woker($node->links);
  $terms_output = art_terms_worker($node->taxonomy);

  $output = $links_output;
  if (!empty($links_output) && !empty($terms_output)) {
    $output .= '&nbsp;|&nbsp;';
  }
  $output .= $terms_output;
  return $output;
}

/*
 * Split out taxonomy terms by vocabulary.
 *
 * @param $terms
 *   An object providing all relevant information for displaying terms:
 *
 * @ingroup themeable
 */
function art_terms_worker($terms) {
  $result = '';
$terms = get_terms_D7($content);
  if (!empty($terms)) {
  ob_start();?>
   <?php
  $result .= ($result == '') ? ob_get_clean() : '&nbsp;|&nbsp;' . ob_get_clean();
  $result .= '<div class="b2-tags">' . render($terms) . '</div>';
  }
  
  return $result;
}

/**
 * Return a themed set of links.
 *
 * @param $links
 *   A keyed array of links to be themed.
 * @param $attributes
 *   A keyed array of attributes
 * @return
 *   A string containing an unordered list of links.
 */
function art_links_woker($links, $attributes = array('class' => 'links')) {
  $output = '';

  if (!empty($links)) {
    $output = '';

    $num_links = count($links);
    $index = 0;

    foreach ($links as $key => $link) {
      $class = $key;
      if (strpos ($class, "read_more") !== FALSE) {
        continue;
      }

      // Automatically add a class to each link and also to each LI
      if (isset($link['attributes']) && isset($link['attributes']['class'])) {
        $link['attributes']['class'] .= ' ' . $key;
      }
      else {
        $link['attributes']['class'] = $key;
      }

      // Add first and last classes to the list of links to help out themers.
      $extra_class = '';
      if ($index == 1) {
        $extra_class .= 'first ';
      }
      if ($index == $num_links) {
        $extra_class .= 'last ';
      }

      $link_output = get_html_link_output($link);
      if (!empty($class)) {
if (strpos ($key, "comment") !== FALSE) {
        
          if ($index > 0 && !empty($link_output) && !empty($output)) {
          $output .= '&nbsp;|&nbsp;';
        }
        ob_start();?>
         <?php
        $output .= ob_get_clean();
        $output .= $link_output;
        $index++;
        continue;
        }
        
if ($index > 0 && !empty($link_output) && !empty($output)) {
          $output .= '&nbsp|&nbsp';
        }
        ob_start();?>
         <?php
        $output .= ob_get_clean();
        $output .= $link_output;
        $index++;
        
      }
      else {
        $output .= '&nbsp;|&nbsp;' . $link_output;
        $index++;
      }
    }
  }

  return $output;
}

function get_html_link_output($link) {
  $output = '';
  // Is the title HTML?
  $html = isset($link['html']) ? $link['html'] : NULL;

  // Initialize fragment and query variables.
  $link['query'] = isset($link['query']) ? $link['query'] : NULL;
  $link['fragment'] = isset($link['fragment']) ? $link['fragment'] : NULL;

  if (isset($link['href'])) {
    if (get_drupal_version() == 5) {
      $output = l($link['title'], $link['href'], $link['attributes'], $link['query'], $link['fragment'], FALSE, $html);
    }
    else {
      $output = l($link['title'], $link['href'], array('language' => $link['language'], 'attributes'=>$link['attributes'], 'query'=>$link['query'], 'fragment'=>$link['fragment'], 'absolute'=>FALSE, 'html'=>$html));
    }
  }
  else if ($link['title']) {
    if (!$html) {
      $link['title'] = check_plain($link['title']);
    }
    $output = $link['title'];
  }

  return $output;
}

function art_content_replace($content) {
  $first_time_str = '<div id="first-time"';
  $article_str = ' class="b2-article"';
  $pos = strpos($content, $first_time_str);
  if($pos !== FALSE)
  {
    $output = str_replace($first_time_str, $first_time_str . $article_str, $content);
    $output = <<< EOT
<div class="b2-post">
        <div class="b2-post-body">
    <div class="b2-post-inner b2-article">
    
<div class="b2-postcontent">
    
      $output

    </div>
    <div class="cleared"></div>
    

    </div>
    
    		<div class="cleared"></div>
        </div>
    </div>
    
EOT;
  }
  else 
  {
    $output = $content;
  }
  return $output;
}

function art_placeholders_output($var1, $var2, $var3) {
  $output = '';
  if (!empty($var1) && !empty($var2) && !empty($var3)) {
    $output .= <<< EOT
      <table class="position" cellpadding="0" cellspacing="0" border="0">
        <tr valign="top">
          <td class="third-width">$var1</td>
          <td class="third-width">$var2</td>
          <td>$var3</td>
        </tr>
      </table>
EOT;
  }
  else if (!empty($var1) && !empty($var2)) {
    $output .= <<< EOT
      <table class="position" cellpadding="0" cellspacing="0" border="0">
        <tr valign="top">
          <td class="third-width">$var1</td>
          <td>$var2</td>
        </tr>
      </table>
EOT;
  }
  else if (!empty($var2) && !empty($var3)) {
    $output .= <<< EOT
      <table class="position" cellpadding="0" cellspacing="0" border="0">
        <tr valign="top">
          <td class="two-thirds-width">$var2</td>
          <td>$var3</td>
        </tr>
      </table>
EOT;
  }
  else if (!empty($var1) && !empty($var3)) {
    $output .= <<< EOT
      <table class="position" cellpadding="0" cellspacing="0" border="0">
        <tr valign="top">
          <td class="half-width">$var1</td>
          <td>$var3</td>
        </tr>
      </table>
EOT;
  }
  else {
    if (!empty($var1)) {
      $output .= <<< EOT
        <div id="var1">$var1</div>
EOT;
    }
    if (!empty($var2)) {
      $output .= <<< EOT
        <div id="var1">$var2</div>
EOT;
    }
    if (!empty($var3)) {
      $output .= <<< EOT
        <div id="var1">$var3</div>
EOT;
    }
  }

  return $output;
}

function art_get_sidebar($sidebar, $vnavigation, $class) {
  $result = 'b2-layout-cell ';
  if (empty($sidebar) && empty($vnavigation)) {
    $result .= 'b2-content';
  }
  else {
    $result .= $class;
  }

  $output = '<div class="'.$result.'">'.render($vnavigation) . render($sidebar).'</div>'; 
  return $output;
}

function art_get_content_cell_style($left, $vnav_left, $right, $vnav_right, $content) {
  return 'b2-layout-cell b2-content'; 
}

function art_submitted_worker($date, $author) {
  $output = '';
  if ($date != '') {
ob_start();?>
     <?php
    $output .= ob_get_clean();
    $output .= $date;
    
  }
  if ($author != '') {
ob_start();?>
     <?php if ($output != '') {
      $output .= '&nbsp;|&nbsp;';
    }
    $output .= ob_get_clean();
    $output .= $author;
    
  }
  return $output;
}

function is_art_links_set($links) {
  $size = sizeof($links);
  if ($size == 0) {
    return FALSE;
  }

  //check if there's "Read more" in node links only  
  $read_more_link = $links['node_read_more'];
  if ($read_more_link != NULL && $size == 1) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Method to define node title output.
 *
*/
function art_node_title_output($title, $node_url, $page) {
  $output = '';
  if (!$page)
    $output = '<a href="' . $node_url . '" title="' . $title . '">' . $title . '</a>';
  else
    $output = $title;
  return $output;
}

function art_vmenu_output($subject, $content) {
  if (empty($content))
    return;

  $output =	art_menu_worker($content, true, 'b2-vmenu');
  $bvm = "<div class=\"b2-vmenublock\">\r\n    <div class=\"b2-vmenublock-body\">\r\n";
  $bvmt = "<div class=\"b2-vmenublockheader\">\r\n    <h3 class=\"t subject\">";
  $evmt = "</h3>\r\n</div>\r\n";
  $bvmc = "<div class=\"b2-vmenublockcontent\">\r\n    <div class=\"b2-vmenublockcontent-body\">\r\n<div class=\"content\">\r\n";
  $evmc = "\r\n</div>\r\n		<div class=\"cleared\"></div>\r\n    </div>\r\n</div>\r\n";
  $evm = "\r\n		<div class=\"cleared\"></div>\r\n    </div>\r\n</div>\r\n";
  echo $bvm;
  if ('' != $bvmt && '' != $evmt && !empty($subject)) {
    echo $bvmt;
    echo $subject;
    echo $evmt;
  }
  echo $bvmc;
  echo $output;
  echo $evmc;
  echo $evm;
}
