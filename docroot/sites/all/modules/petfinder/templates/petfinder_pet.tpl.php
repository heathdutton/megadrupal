<?php
/**
 * @file
 * 
 * Default theme implementation to display a pet record from Petfinder.com.
 * For more information on the fields returned, visit:
 * 
 * http://www.petfinder.com/developers/api-docs
 * http://api.petfinder.com/schemas/0.9/petfinder.xsd
 *
 * Available variables:
 * - $pet: The complete pet record from Petfinder.com
 * - $page: Flag for the page state
 *
 * Most other standard page-level variables are also available at this level
 */

//_petfinder_dpm($variables);
?>
<div class="<?php print $classes; ?> petfinder-pet-<?php print $pet['id']; ?>">

  <?php if ($page) : ?>
    <?php if (!empty($pet['media']['photos']['photo'][PETFINDER_IMG_PN])) : ?>
    <div class="petfinder-pet-photo-main">
      <img src="<?php print $pet['media']['photos']['photo'][PETFINDER_IMG_PN]; ?>">
    </div>
    <?php endif; ?>
    
    <?php if (!$page) : ?>
    <div class="petfinder-pet-name">
      <?php print $pet['name']; ?>
    </div>
    <?php endif; ?>
    
    <?php if (!empty($pet['description'])) : ?>
    <div class="petfinder-pet-description">
      <?php print $pet['description']; ?>
    </div>
    <?php endif; ?>
    
    <?php if (!empty($pet['options'])) : ?>
    <div class="petfinder-pet-options">
      <?php print $pet['options']; ?>
    </div>
    <?php endif; ?>
    
    <?php if (!empty($pet['breeds']['breed'])) : ?>
    <div class="petfinder-pet-breeds">
      <div class="label">Breed(s):</div>
      <div class="info"><?php print $pet['breeds']['breed']; ?></div>
    </div>
    <?php endif; ?>
    
    <?php if (!empty($pet['sex'])) : ?>
    <div class="petfinder-pet-sex">
      <div class="label">Gender:</div>
      <div class="info"><?php print $pet['sex']; ?></div>
    </div>
    <?php endif; ?>
    
    <?php if (!empty($pet['age'])) : ?>
    <div class="petfinder-pet-age">
      <div class="label">Age:</div>
      <div class="info"><?php print $pet['age']; ?></div>
    </div>
    <?php endif; ?>
    
    <?php if (!empty($pet['size'])) : ?>
    <div class="petfinder-pet-size">
      <div class="label">Size:</div>
      <div class="info"><?php print $pet['size']; ?></div>
    </div>
    <?php endif; ?>
    
    <?php if (!empty($shelter)) : ?>
    <div class="petfinder-pet-contact">
      <?php print $shelter; ?>
    </div>
    <?php endif; ?>
    
    <?php if (!empty($pet['shelterPetId'])) : ?>
    <div class="petfinder-pet-shelter-pet-id">
      <div class="label">Shelter Pet ID:</div>
      <div class="info"><?php print $pet['shelterPetId']; ?></div>
    </div>
    <?php endif; ?>
    
    <?php if (!empty($pet['media']['photos']['photo'])) : ?>
    <div class="petfinder-pet-media">
      <?php
      // Add colorbox support if installed
      $link_attrs = (module_exists('colorbox')) ? array('class' => 'colorbox-load', 'rel' => 'petfinder-pet-media-gallery') : array('target' => '_petfinder_photo');
      for ($i = PETFINDER_IMG_FPM; $i < count($pet['media']['photos']['photo']); $i += 5) {
        print l(
          '<img src="' . $pet['media']['photos']['photo'][$i] . '">', 
          $pet['media']['photos']['photo'][$i - PETFINDER_IMG_FPM],
          array(
            'html' => TRUE,
            'attributes' => $link_attrs,
          )
        );
      }
      ?>
    </div>
    <?php endif; ?>

  <?php else: ?>

    <?php if (!empty($pet['media']['photos']['photo'][PETFINDER_IMG_FPM])) : ?>
    <div class="petfinder-pet-photo-main">
      <?php
      print l(
        '<img src="' . $pet['media']['photos']['photo'][PETFINDER_IMG_FPM] . '">',
        'petfinder/pet/' . $pet['id'],
        array('html' => TRUE)
      );
      ?>
    </div>
    <?php endif; ?>
    
    <div class="petfinder-pet-details">
      <?php if (!$page) : ?>
      <div class="petfinder-pet-name">
        <?php print l($pet['name'], 'petfinder/pet/' . $pet['id']); ?>
      </div>
      <?php endif; ?>
      
      <?php if (!empty($pet['breeds']['breed'])) : ?>
      <div class="petfinder-pet-breeds">
        <div class="label">Breed(s):</div>
        <div class="info"><?php print $pet['breeds']['breed']; ?></div>
      </div>
      <?php endif; ?>
      
      <?php if (!empty($pet['sex'])) : ?>
      <div class="petfinder-pet-sex">
        <div class="label">Gender:</div>
        <div class="info"><?php print $pet['sex']; ?></div>
      </div>
      <?php endif; ?>
      
      <?php if (!empty($pet['age'])) : ?>
      <div class="petfinder-pet-age">
        <div class="label">Age:</div>
        <div class="info"><?php print $pet['age']; ?></div>
      </div>
      <?php endif; ?>
    </div>

  <?php endif; ?>
</div>
