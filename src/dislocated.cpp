/*
 * Licensed to Cloudkick, Inc ('Cloudkick') under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * Cloudkick licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "dl_server.h"
#include "dl_tunables.h"
#include "dl_version.h"
#include "dl_core.h"

static void show_version()
{
#ifdef DL_IS_DEV
  const char *extra = "-dev";
#else
  const char *extra = "";
#endif
  fprintf(stdout, "dislocated - %d.%d.%d%s\n", DL_VERSION_MAJOR,
          DL_VERSION_MINOR, DL_VERSION_PATCH, extra);
  exit(EXIT_SUCCESS);
}

static void show_help()
{
  fprintf(stdout, "dislocated - Distributed Service Locater\n");
  fprintf(stdout, "  Usage:  \n");
  fprintf(stdout, "    dislocated [-h|-V]\n");
  fprintf(stdout, "\n");
  fprintf(stdout, "     -h          Show Help message\n");
  fprintf(stdout, "     -V          Show Version number\n");
  exit(EXIT_SUCCESS);
}


static void error_out(const char *msg) {
  fprintf(stderr, "ERROR: %s\n\n", msg);
  exit(EXIT_FAILURE);
}

static dl_serv_conf_t* default_config()
{
  dl_serv_conf_t *conf = new dl_serv_conf_t();
  return conf;
}

int main(int argc, char *const *argv)
{
  int c;
  int rc;

  dl_serv_conf_t *conf = default_config();
  while ((c = getopt(argc, argv, "hV")) != -1) {
    switch (c) {
      case 'V':
        show_version();
        break;
      case 'h':
        show_help();
        break;
      case '?':
        error_out("See -h for correct options");
        break;
    }
  }

  dlCore *core = new dlCore();

  rc = core->config(conf);
  if (!rc) {
    error_out("Error from configuring core");
  }
  core->shutdown();

  delete core;
  delete conf;
  exit(EXIT_SUCCESS);
}
