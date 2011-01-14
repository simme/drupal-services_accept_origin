<?php
// $Id$

/**
 * @file
 *  Authentication callback and settings forms
 */

/**
 * Display the settings form for the endpoint
 */
function _services_accept_origin_settings($settings) {
  $form = array();

  // Breaking up translatable strings with new line as suggested here:
  // http://drupal.org/node/976620#comment-3922940 by kiamlaluno
  $whitelist = !empty($settings['whitelist']) ? $settings['whitelist'] : '';
  $form['whitelist'] = array(
    '#description'    => t('Enter the domains you want to whitelist for
      this endpoint. One per line, remember http://. If you want to allow
      all requests, just enter "*".'),
    '#title'          => t('Accept origin whitelist'),
    '#type'           => 'textarea',
    '#default_value'  => $whitelist,
  );

  $nop = (bool)$settings['no_origin_policy'];
  $form['no_origin_policy'] = array(
    '#description'    => t('Unchecking this box means that all requests
      without an ORIGIN header will be allowed access. Checking the box
      will essentially deny all requests other then XMLHttpRequest unless
      the header is explicitly set by the client.'),
    '#title'          => t('Allow access to requests without ORIGIN header'),
    '#type'           => 'checkbox',
    '#default_value'  => $nop,
  );

  return $form;
}

function _services_accept_origin_controller_settings($settings, $controller, $endpoint, $class, $name) {
  // Services crashes without this one implemented.
  // Coming soon... perhaps.
}

/**
 * Authenticates a call by checking the origin agains the whitelist.
 *
 * @param $settings
 *  The settings for the authentication module.
 * @param $method
 *  The method that's being called
 * @param $args
 *  The arguments that are being used to call the method
 * @return
 *  Returns nothing, or a error message if authentication fails
 */
function _services_accept_origin_authenticate($settings, $method, $args) {
  $whitelist = explode("\n", $settings['whitelist']);
  $origin = !empty($_SERVER['HTTP_ORIGIN']) ? $_SERVER['HTTP_ORIGIN'] : FALSE;
  $origin_is_whitelisted = FALSE;

  // No ORIGIN header and should let it pass
  if (!$origin && $settings['no_origin_policy']) {
    $origin_is_whitelisted = TRUE;
    $origin = '*';
  }

  // Check for wildcard, if found, let all through
  elseif (strpos($settings['whitelist'], '*') !== FALSE) {
    $origin_is_whitelisted = TRUE;
  }

  // Loop through whitelist and compare origin
  else {
    // Determine if the domain is whitelisted, compensates
    // for traling slashs and stuff
    foreach ($whitelist as $domain) {
      if (strpos($domain, $origin) === 0) {
        $origin_is_whitelisted = TRUE;
        break;
      }
    }
  }

  // Send headers and let request through since we got
  // an origin and it's in the list
  if ($origin_is_whitelisted) {
    drupal_set_header('Access-Control-Allow-Origin: ' . $origin);
  }
  // Deny access
  else {
    return t('Unauthorized origin.');
  }
}