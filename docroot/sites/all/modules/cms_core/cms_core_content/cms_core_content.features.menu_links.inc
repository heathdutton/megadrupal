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
    'weight' => -46,
    'customized' => 1,
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
    'weight' => -45,
    'customized' => 1,
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Blog');
  t('Blog Classic Style');
  t('Blog Grid 2 Columns');
  t('Blog Grid 3 Columns');
  t('Blog Grid 4 Columns');
  t('Blog Modern Style');
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

  return $menu_links;
}
