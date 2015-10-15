<?php
/**
 * @file
 * cms_core_content.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function cms_core_content_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu_blog-classic-style:blog
  $menu_links['main-menu_blog-classic-style:blog'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'blog',
    'router_path' => 'blog',
    'link_title' => 'Blog Classic Style',
    'options' => array(
      'identifier' => 'main-menu_blog-classic-style:blog',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'main-menu_blog:blog',
  );
  // Exported menu link: main-menu_blog-grid-2-columns:blog/grid2
  $menu_links['main-menu_blog-grid-2-columns:blog/grid2'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'blog/grid2',
    'router_path' => 'blog/grid2',
    'link_title' => 'Blog Grid 2 Columns',
    'options' => array(
      'identifier' => 'main-menu_blog-grid-2-columns:blog/grid2',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'parent_identifier' => 'main-menu_blog:blog',
  );
  // Exported menu link: main-menu_blog-grid-3-columns:blog/grid3
  $menu_links['main-menu_blog-grid-3-columns:blog/grid3'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'blog/grid3',
    'router_path' => 'blog/grid3',
    'link_title' => 'Blog Grid 3 Columns',
    'options' => array(
      'identifier' => 'main-menu_blog-grid-3-columns:blog/grid3',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
    'parent_identifier' => 'main-menu_blog:blog',
  );
  // Exported menu link: main-menu_blog-grid-4-columns:blog/grid4
  $menu_links['main-menu_blog-grid-4-columns:blog/grid4'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'blog/grid4',
    'router_path' => 'blog/grid4',
    'link_title' => 'Blog Grid 4 Columns',
    'options' => array(
      'identifier' => 'main-menu_blog-grid-4-columns:blog/grid4',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
    'parent_identifier' => 'main-menu_blog:blog',
  );
  // Exported menu link: main-menu_blog-modern-style:blog/modern
  $menu_links['main-menu_blog-modern-style:blog/modern'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'blog/modern',
    'router_path' => 'blog/modern',
    'link_title' => 'Blog Modern Style',
    'options' => array(
      'identifier' => 'main-menu_blog-modern-style:blog/modern',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'main-menu_blog:blog',
  );
  // Exported menu link: main-menu_blog:blog
  $menu_links['main-menu_blog:blog'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'blog',
    'router_path' => 'blog',
    'link_title' => 'Blog',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_blog:blog',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -48,
    'customized' => 1,
  );
  // Exported menu link: main-menu_elements:node/24
  $menu_links['main-menu_elements:node/24'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/24',
    'router_path' => 'uuid',
    'link_title' => 'Elements',
    'options' => array(
      'identifier' => 'main-menu_elements:node/24',
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -50,
    'customized' => 0,
  );
  // Exported menu link: main-menu_icon--content-boxes:uuid/node/0092815b-c9df-4b09-9441-3d78387eaaef
  $menu_links['main-menu_icon--content-boxes:uuid/node/0092815b-c9df-4b09-9441-3d78387eaaef'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'uuid/node/0092815b-c9df-4b09-9441-3d78387eaaef',
    'router_path' => 'uuid',
    'link_title' => 'Icon / Content Boxes',
    'options' => array(
      'identifier' => 'main-menu_icon--content-boxes:uuid/node/0092815b-c9df-4b09-9441-3d78387eaaef',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
    'uuid' => '0092815b-c9df-4b09-9441-3d78387eaaef',
    'vuuid' => 'df2a64ff-ba29-4c0e-83a9-8017cb89a6af',
    'parent_identifier' => 'main-menu_elements:node/24',
  );
  // Exported menu link: main-menu_icons:uuid/node/478fa672-ec35-4111-8ecc-5309f3180243
  $menu_links['main-menu_icons:uuid/node/478fa672-ec35-4111-8ecc-5309f3180243'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'uuid/node/478fa672-ec35-4111-8ecc-5309f3180243',
    'router_path' => 'uuid',
    'link_title' => 'Icons',
    'options' => array(
      'identifier' => 'main-menu_icons:uuid/node/478fa672-ec35-4111-8ecc-5309f3180243',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -26,
    'customized' => 0,
    'uuid' => '478fa672-ec35-4111-8ecc-5309f3180243',
    'vuuid' => '5be02a28-beac-47d5-bb30-b8e4ffa3d864',
    'parent_identifier' => 'main-menu_elements:node/24',
  );
  // Exported menu link: main-menu_portfolio-1-column-with-excerpt:portfolio/1excerpt
  $menu_links['main-menu_portfolio-1-column-with-excerpt:portfolio/1excerpt'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/1excerpt',
    'router_path' => 'portfolio/1excerpt',
    'link_title' => 'Portfolio 1 Column with Excerpt',
    'options' => array(
      'identifier' => 'main-menu_portfolio-1-column-with-excerpt:portfolio/1excerpt',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -41,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-1-column:portfolio/1
  $menu_links['main-menu_portfolio-1-column:portfolio/1'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/1',
    'router_path' => 'portfolio/1',
    'link_title' => 'Portfolio 1 Column',
    'options' => array(
      'identifier' => 'main-menu_portfolio-1-column:portfolio/1',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-2-columns:portfolio/2
  $menu_links['main-menu_portfolio-2-columns:portfolio/2'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/2',
    'router_path' => 'portfolio/2',
    'link_title' => 'Portfolio 2 Columns',
    'options' => array(
      'identifier' => 'main-menu_portfolio-2-columns:portfolio/2',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-3-columns:portfolio/3
  $menu_links['main-menu_portfolio-3-columns:portfolio/3'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/3',
    'router_path' => 'portfolio/3',
    'link_title' => 'Portfolio 3 Columns',
    'options' => array(
      'identifier' => 'main-menu_portfolio-3-columns:portfolio/3',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-6-columns:portfolio/6
  $menu_links['main-menu_portfolio-6-columns:portfolio/6'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/6',
    'router_path' => 'portfolio/6',
    'link_title' => 'Portfolio 6 Columns',
    'options' => array(
      'identifier' => 'main-menu_portfolio-6-columns:portfolio/6',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-masonry-2-with-excerpt:portfolio/v2excerpt
  $menu_links['main-menu_portfolio-masonry-2-with-excerpt:portfolio/v2excerpt'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/v2excerpt',
    'router_path' => 'portfolio/v2excerpt',
    'link_title' => 'Portfolio Masonry 2 with Excerpt',
    'options' => array(
      'identifier' => 'main-menu_portfolio-masonry-2-with-excerpt:portfolio/v2excerpt',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -40,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-masonry-2:portfolio/v2
  $menu_links['main-menu_portfolio-masonry-2:portfolio/v2'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/v2',
    'router_path' => 'portfolio/v2',
    'link_title' => 'Portfolio Masonry 2',
    'options' => array(
      'identifier' => 'main-menu_portfolio-masonry-2:portfolio/v2',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -45,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-masonry-3-with-excerpt:portfolio/v3excerpt
  $menu_links['main-menu_portfolio-masonry-3-with-excerpt:portfolio/v3excerpt'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/v3excerpt',
    'router_path' => 'portfolio/v3excerpt',
    'link_title' => 'Portfolio Masonry 3 with Excerpt',
    'options' => array(
      'identifier' => 'main-menu_portfolio-masonry-3-with-excerpt:portfolio/v3excerpt',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -39,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-masonry-3:portfolio/v3
  $menu_links['main-menu_portfolio-masonry-3:portfolio/v3'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/v3',
    'router_path' => 'portfolio/v3',
    'link_title' => 'Portfolio Masonry 3',
    'options' => array(
      'identifier' => 'main-menu_portfolio-masonry-3:portfolio/v3',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -44,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-masonry-4-with-excerpt:portfolio/v4excerpt
  $menu_links['main-menu_portfolio-masonry-4-with-excerpt:portfolio/v4excerpt'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/v4excerpt',
    'router_path' => 'portfolio/v4excerpt',
    'link_title' => 'Portfolio Masonry 4 with Excerpt',
    'options' => array(
      'identifier' => 'main-menu_portfolio-masonry-4-with-excerpt:portfolio/v4excerpt',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -38,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-masonry-4:portfolio/v4
  $menu_links['main-menu_portfolio-masonry-4:portfolio/v4'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/v4',
    'router_path' => 'portfolio/v4',
    'link_title' => 'Portfolio Masonry 4',
    'options' => array(
      'identifier' => 'main-menu_portfolio-masonry-4:portfolio/v4',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -43,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-masonry-6:portfolio/v6
  $menu_links['main-menu_portfolio-masonry-6:portfolio/v6'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/v6',
    'router_path' => 'portfolio/v6',
    'link_title' => 'Portfolio Masonry 6',
    'options' => array(
      'identifier' => 'main-menu_portfolio-masonry-6:portfolio/v6',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -42,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-vertical-2-with-excerpt:portfolio/v2excerpt
  $menu_links['main-menu_portfolio-vertical-2-with-excerpt:portfolio/v2excerpt'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/v2excerpt',
    'router_path' => 'portfolio/v2excerpt',
    'link_title' => 'Portfolio Vertical 2 with Excerpt',
    'options' => array(
      'identifier' => 'main-menu_portfolio-vertical-2-with-excerpt:portfolio/v2excerpt',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -40,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-vertical-2:portfolio/v2
  $menu_links['main-menu_portfolio-vertical-2:portfolio/v2'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/v2',
    'router_path' => 'portfolio/v2',
    'link_title' => 'Portfolio Vertical 2',
    'options' => array(
      'identifier' => 'main-menu_portfolio-vertical-2:portfolio/v2',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -45,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-vertical-3-with-excerpt:portfolio/v3excerpt
  $menu_links['main-menu_portfolio-vertical-3-with-excerpt:portfolio/v3excerpt'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/v3excerpt',
    'router_path' => 'portfolio/v3excerpt',
    'link_title' => 'Portfolio Vertical 3 with Excerpt',
    'options' => array(
      'identifier' => 'main-menu_portfolio-vertical-3-with-excerpt:portfolio/v3excerpt',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -39,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-vertical-3:portfolio/v3
  $menu_links['main-menu_portfolio-vertical-3:portfolio/v3'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/v3',
    'router_path' => 'portfolio/v3',
    'link_title' => 'Portfolio Vertical 3',
    'options' => array(
      'identifier' => 'main-menu_portfolio-vertical-3:portfolio/v3',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -44,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-vertical-4-with-excerpt:portfolio/v4excerpt
  $menu_links['main-menu_portfolio-vertical-4-with-excerpt:portfolio/v4excerpt'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/v4excerpt',
    'router_path' => 'portfolio/v4excerpt',
    'link_title' => 'Portfolio Vertical 4 with Excerpt',
    'options' => array(
      'identifier' => 'main-menu_portfolio-vertical-4-with-excerpt:portfolio/v4excerpt',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -38,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-vertical-4:portfolio/v4
  $menu_links['main-menu_portfolio-vertical-4:portfolio/v4'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/v4',
    'router_path' => 'portfolio/v4',
    'link_title' => 'Portfolio Vertical 4',
    'options' => array(
      'identifier' => 'main-menu_portfolio-vertical-4:portfolio/v4',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -43,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio-vertical-6:portfolio/v6
  $menu_links['main-menu_portfolio-vertical-6:portfolio/v6'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio/v6',
    'router_path' => 'portfolio/v6',
    'link_title' => 'Portfolio Vertical 6',
    'options' => array(
      'identifier' => 'main-menu_portfolio-vertical-6:portfolio/v6',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -42,
    'customized' => 1,
    'parent_identifier' => 'main-menu_portfolio:portfolio',
  );
  // Exported menu link: main-menu_portfolio:portfolio
  $menu_links['main-menu_portfolio:portfolio'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'portfolio',
    'router_path' => 'portfolio',
    'link_title' => 'Portfolio',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_portfolio:portfolio',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -47,
    'customized' => 1,
  );
  // Exported menu link: main-menu_responsive-columns:uuid/node/4960342e-40d4-438d-bf0b-886629bcd261
  $menu_links['main-menu_responsive-columns:uuid/node/4960342e-40d4-438d-bf0b-886629bcd261'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'uuid/node/4960342e-40d4-438d-bf0b-886629bcd261',
    'router_path' => 'uuid',
    'link_title' => 'Responsive columns',
    'options' => array(
      'identifier' => 'main-menu_responsive-columns:uuid/node/4960342e-40d4-438d-bf0b-886629bcd261',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
    'uuid' => '4960342e-40d4-438d-bf0b-886629bcd261',
    'vuuid' => 'c16179ce-d2c8-4d1e-8716-5e3161ed11e5',
    'parent_identifier' => 'main-menu_elements:node/24',
  );
  // Exported menu link: main-menu_social-icons:uuid/node/477c5d73-b0a9-47ee-afe0-6fc2777ad322
  $menu_links['main-menu_social-icons:uuid/node/477c5d73-b0a9-47ee-afe0-6fc2777ad322'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'uuid/node/477c5d73-b0a9-47ee-afe0-6fc2777ad322',
    'router_path' => 'uuid',
    'link_title' => 'Social icons',
    'options' => array(
      'identifier' => 'main-menu_social-icons:uuid/node/477c5d73-b0a9-47ee-afe0-6fc2777ad322',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
    'uuid' => '477c5d73-b0a9-47ee-afe0-6fc2777ad322',
    'vuuid' => '97b33ada-f621-4d17-8d99-417596377d2b',
    'parent_identifier' => 'main-menu_elements:node/24',
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Blog');
  t('Blog Classic Style');
  t('Blog Grid 2 Columns');
  t('Blog Grid 3 Columns');
  t('Blog Grid 4 Columns');
  t('Blog Modern Style');
  t('Elements');
  t('Icon / Content Boxes');
  t('Icons');
  t('Portfolio');
  t('Portfolio 1 Column');
  t('Portfolio 1 Column with Excerpt');
  t('Portfolio 2 Columns');
  t('Portfolio 3 Columns');
  t('Portfolio 6 Columns');
  t('Portfolio Masonry 2');
  t('Portfolio Masonry 2 with Excerpt');
  t('Portfolio Masonry 3');
  t('Portfolio Masonry 3 with Excerpt');
  t('Portfolio Masonry 4');
  t('Portfolio Masonry 4 with Excerpt');
  t('Portfolio Masonry 6');
  t('Portfolio Vertical 2');
  t('Portfolio Vertical 2 with Excerpt');
  t('Portfolio Vertical 3');
  t('Portfolio Vertical 3 with Excerpt');
  t('Portfolio Vertical 4');
  t('Portfolio Vertical 4 with Excerpt');
  t('Portfolio Vertical 6');
  t('Responsive columns');
  t('Social icons');

  return $menu_links;
}
