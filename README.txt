Services Accept Origin
----------------------

This is an authentication module for Services (http://drupal.org/project/services).

It let's you specify a whitelist of domains. Domains in this whitelist will then
be allowed to make cross domain XMLHttpRequests. This works by setting the
*Access-Control-Allow-Origin* header to match the origin of the requester.

We also perform a server side check, and if the domain is not present in the
white list the request is denied access directly, which saves your server
resources.

*Tested with Services 3.x and RESTServer*

# Roadmap

  *  Implement the entire HTTP Access Control specification. See https://developer.mozilla.org/En/HTTP_access_control
  *  Implement WhiteList presets which then be associated with a specific resrouce,
     allowing different resources to have different whitelists.
  *  Integrate with services_oauth to set a whitelisted domain per consumer.
  *  RegExp domains to allow all subdomains from a specific origin