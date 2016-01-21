<?php

/**
 * @file
 * Template file for SocialMediaBar rendered output.
 */

?>
<div class="socialmediabar-container">
    <div class="socialmediabar">
        <div class="total-container container">
            <div class="count"></div>
            Shares
        </div>
        <?php if (isset($networks['linkedin'])): ?>
        <div class="network-container container linkedin" data-network="linkedin">
            <button class="share-button">Linked In</button>
            <div class="count"></div>
        </div>
        <?php endif; ?>
        <?php if (isset($networks['twitter'])): ?>
        <div class="network-container container twitter" data-network="twitter">
            <button class="share-button">Twitter</button>
            <div class="count"></div>
        </div>
        <?php endif; ?>
        <?php if (isset($networks['googleplus'])): ?>
        <div class="network-container container googleplus" data-network="googleplus">
            <button class="share-button">Google Plus</button>
            <div class="count"></div>
        </div>
        <?php endif; ?>
        <?php if (isset($networks['facebook'])): ?>
            <div class="network-container container facebook" data-network="facebook">
                <button class="share-button">Facebook</button>
                <div class="count"></div>
            </div>
        <?php endif; ?>
        <?php if (isset($networks['email']) && !empty($api_key)): ?>
        <div class="network-container container email" data-network="email">
            <span class='st_email' displayText='Email'></span>
            <div class="count"></div>
        </div>
        <?php endif; ?>
    </div>
</div>
