<?php
/**
 * @file
 * openacademy_core_demo.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function openacademy_core_demo_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'openacademy_about';
  $page->task = 'page';
  $page->admin_title = 'About Page';
  $page->admin_description = 'This is the about page of the site used to show off information about this site';
  $page->path = 'about';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'access content',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'normal',
    'title' => 'About',
    'name' => 'menu-header-menu',
    'weight' => '-10',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_openacademy_about_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'openacademy_about';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'About Page',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'css_id' => 'about-panel',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'sutro';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'About';
  $display->uuid = '4796154d-368d-11b4-fdda-25805158a32f';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-0a3c59dd-ef5c-33f4-653f-17426857ea37';
    $pane->panel = 'column1';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => '',
      'title' => 'Elephant Foot yam moth',
      'body' => 'Napa cabbage chinese artichoke land cress shallot kohlrabi chard tomatillo celtuce. Paracress garden rocket indian pea celery, epazote, earthnut pea ahipa. Mooli pignut kohlrabi kale yarrow winter purslane sweet pepper land cress. Green bean bamboo shoot kohlrabi sierra leone bologi lotus root beet greens.

Ahipa chickpea catsear - fat hen sweet corn aka corn; aka maize, radicchio nopal - swede - bitter melon. Radicchio, spinach fiddlehead. Potato, scallion fluted pumpkin - prussian asparagus earthnut pea pak choy west indian gherkin - tigernut? Manioc sweet pepper - ceylon spinach, courgette pea, spring onion pumpkin, jerusalem artichoke common bean manioc.',
      'format' => 'plain_text',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '0a3c59dd-ef5c-33f4-653f-17426857ea37';
    $display->content['new-0a3c59dd-ef5c-33f4-653f-17426857ea37'] = $pane;
    $display->panels['column1'][0] = 'new-0a3c59dd-ef5c-33f4-653f-17426857ea37';
    $pane = new stdClass();
    $pane->pid = 'new-42623a7e-df6b-8184-edb5-b9dcadd88370';
    $pane->panel = 'column2';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => '',
      'title' => 'Jícama peanut sweet corn',
      'body' => 'Daikon bamboo shoot chinese mallow pea sprouts spinach runner bean arracacha new zealand spinach. Brussels sprout potato greater plantain, guar. Horse gram pak choy prussian asparagus broadleaf arrowhead spring onion polk sweet potato or kumara.

Plectranthus tinda tomatillo potato; mooli horseradish. Celtuce pea sprouts fluted pumpkin tatsoi spinach; jícama okra velvet bean. Chard, urad bean yardlong bean scorzonera, winged bean. Mung bean fava bean, pumpkin tarwi. Hamburg parsley fat hen, ceylon spinach earthnut pea tomato, indian pea welsh onion yarrow, broadleaf arrowhead avocado chickweed, broadleaf arrowhead.',
      'format' => 'plain_text',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '42623a7e-df6b-8184-edb5-b9dcadd88370';
    $display->content['new-42623a7e-df6b-8184-edb5-b9dcadd88370'] = $pane;
    $display->panels['column2'][0] = 'new-42623a7e-df6b-8184-edb5-b9dcadd88370';
    $pane = new stdClass();
    $pane->pid = 'new-c6b60970-26a5-b7e4-49a6-37862fc992a2';
    $pane->panel = 'footer';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => '',
      'title' => 'Bell pepper sierra leone bologi -',
      'body' => 'Urad bean, broccoli rabe bell pepper polk napa cabbage wild leek sea beet. Celeriac catsear fluted pumpkin garlic golden samphire, tomatillo zucchini, sea beet ahipa winged bean. Fiddlehead chickpea ginger. Swiss chard earthnut pea florence fennel chaya - kurrat lima bean sweet potato or kumara.

Bell pepper sierra leone bologi - squash watercress gumbo. Plectranthus tomatillo cauliflower, cardoon lentil kurrat bell pepper prairie turnip cassava pigeon pea, elephant foot yam water spinach. Polk leek dolichos bean, paracress sea kale, tinda; lamb\'s lettuce, manioc salsify; sorrel. Fat hen - cauliflower camas greater plantain sierra leone bologi chinese artichoke turnip greens golden samphire pignut peanut epazote greater plantain.
',
      'format' => 'plain_text',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'c6b60970-26a5-b7e4-49a6-37862fc992a2';
    $display->content['new-c6b60970-26a5-b7e4-49a6-37862fc992a2'] = $pane;
    $display->panels['footer'][0] = 'new-c6b60970-26a5-b7e4-49a6-37862fc992a2';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['openacademy_about'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'openacademy_contact';
  $page->task = 'page';
  $page->admin_title = 'Contact Page';
  $page->admin_description = 'This is the contact page of the site used to tell people about the department!';
  $page->path = 'contact';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'access content',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Contact',
    'name' => 'menu-header-menu',
    'weight' => '-5',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '-14',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_openacademy_contact_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'openacademy_contact';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Contact Page',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'css_id' => 'contact-panel',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'sutro';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Contact';
  $display->uuid = '18c7e781-d0a9-61d4-89a3-7e3e90227953';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-d55946a3-227b-f854-fd7e-106e69e81187';
    $pane->panel = 'column1';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => '',
      'title' => 'Elephant Foot yam moth',
      'body' => 'Napa cabbage chinese artichoke land cress shallot kohlrabi chard tomatillo celtuce. Paracress garden rocket indian pea celery, epazote, earthnut pea ahipa. Mooli pignut kohlrabi kale yarrow winter purslane sweet pepper land cress. Green bean bamboo shoot kohlrabi sierra leone bologi lotus root beet greens.

Ahipa chickpea catsear - fat hen sweet corn aka corn; aka maize, radicchio nopal - swede - bitter melon. Radicchio, spinach fiddlehead. Beet greens, winged bean canna elephant foot yam bitter melon jícama courgette? Bamboo shoot, green bean melokhia scallion napa cabbage new zealand spinach hamburg parsley, earthnut pea burdock. Potato, scallion fluted pumpkin - prussian asparagus earthnut pea pak choy west indian gherkin - tigernut? Manioc sweet pepper - ceylon spinach, courgette pea, spring onion pumpkin, jerusalem artichoke common bean manioc.',
      'format' => 'plain_text',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'd55946a3-227b-f854-fd7e-106e69e81187';
    $display->content['new-d55946a3-227b-f854-fd7e-106e69e81187'] = $pane;
    $display->panels['column1'][0] = 'new-d55946a3-227b-f854-fd7e-106e69e81187';
    $pane = new stdClass();
    $pane->pid = 'new-61777bd1-66ca-7104-b9e9-dc54d265fd07';
    $pane->panel = 'column2';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => '',
      'title' => 'Jícama peanut sweet corn',
      'body' => 'Daikon bamboo shoot chinese mallow pea sprouts spinach runner bean arracacha new zealand spinach. Brussels sprout potato greater plantain, guar. Horse gram pak choy prussian asparagus broadleaf arrowhead spring onion polk sweet potato or kumara.

Plectranthus tinda tomatillo potato; mooli horseradish. Celtuce pea sprouts fluted pumpkin tatsoi spinach; jícama okra velvet bean. Broadleaf arrowhead.

Jícama peanut sweet corn aka corn; aka maize common bean mustard, prussian asparagus chinese cabbage canna sorrel, avocado winter purslane. Ulluco beet greens lettuce bitter melon, bitterleaf taro celtuce.
',
      'format' => 'plain_text',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '61777bd1-66ca-7104-b9e9-dc54d265fd07';
    $display->content['new-61777bd1-66ca-7104-b9e9-dc54d265fd07'] = $pane;
    $display->panels['column2'][0] = 'new-61777bd1-66ca-7104-b9e9-dc54d265fd07';
    $pane = new stdClass();
    $pane->pid = 'new-05987277-0a8b-ea74-f996-e9dcad83acb5';
    $pane->panel = 'footer';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => '',
      'title' => 'Bell pepper sierra leone bologi -',
      'body' => 'Urad bean, broccoli rabe bell pepper polk napa cabbage wild leek sea beet. Celeriac catsear fluted pumpkin garlic golden samphire, tomatillo zucchini, sea beet ahipa winged bean. Fiddlehead chickpea ginger. Swiss chard earthnut pea florence fennel chaya - kurrat lima bean sweet potato or kumara.

Bell pepper sierra leone bologi - squash watercress gumbo. Plectranthus tomatillo cauliflower, cardoon lentil kurrat bell pepper prairie turnip cassava pigeon pea, elephant foot yam water spinach. Polk leek dolichos bean, paracress sea kale, tinda; lamb\'s lettuce.',
      'format' => 'plain_text',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '05987277-0a8b-ea74-f996-e9dcad83acb5';
    $display->content['new-05987277-0a8b-ea74-f996-e9dcad83acb5'] = $pane;
    $display->panels['footer'][0] = 'new-05987277-0a8b-ea74-f996-e9dcad83acb5';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['openacademy_contact'] = $page;

  return $pages;

}
