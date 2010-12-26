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

  $whitelist = !empty($settings['whitelist']) ? $settings['whitelist'] : '';
  $form['whitelist'] = array(
    '#description'    => t('Enter the domains you want to whitelist for ' .
      'this endpoint. One per line, remember http://. If you want to allow ' .
      'all requests, just enter "*".'),
    '#title'          => t('Accept Origin Whitelist'),
    '#type'           => 'textarea',
    '#default_value'  => $whitelist,
  );

  $nop = (bool)$settings['no_origin_policy'];
  $form['no_origin_policy'] = array(
    '#description'    => t('Unchecking this box means that all requests ' .
      'without an ORIGIN header will be allowed access. Checking the box ' .
      'will essentially deny all requests other then XMLHttpRequest unless ' .
      'the header is explicitly set by the client.'),
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
 * @param     array         $settings
  *  The settings for the authentication module.
 * @param     array         $method
 *  The method that's being called
 * @param     array         $args
 *  The arguments that are being used to call the method
 * @return    void|string
 *  Returns nothing, or a error message if authentication fails
 */
function _services_accept_origin_authenticate($settings, $method, $args) {
  $whitelist = explode("\n", $settings['whitelist']);
  $origin = !empty($_SERVER['HTTP_ORIGIN']) ? $_SERVER['HTTP_ORIGIN'] : FALSE;
  $origin_is_whitelisted = FALSE;

  // Check no origin policy
  if (!$origin) {
    if ($settings['no_origin_policy']) {
      return;
    }
    else {
      return 'Unauthorized origin.';
    }
  }

  // Determine if the domain is whitelisted, compensates
  // for traling slashs and stuff
  foreach ($whitelist as $domain) {
    // Make star always pass requests through
    if ($domain == '*') {
      $origin_is_whitelisted = TRUE;
      $origin = $domain;
      break;
    }
    elseif (strpos($domain, $origin) === 0) {
      $origin_is_whitelisted = TRUE;
      break;
    }
  }

  // Send headers and let request through since we got
  // an origin and it's in the list
  if ($origin_is_whitelisted) {
    drupal_set_header('Access-Control-Allow-Origin: ' . $origin);
  }
  else {
    // Returned as an HTTP-header by Services.
    // Thus it should not be translateable.
    return 'Unauthorized origin.';
  }
}