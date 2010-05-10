# Licensed to Cloudkick, Inc ('Cloudkick') under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# Cloudkick licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


EnsureSConsVersion(1, 1, 0)

import os
import re
from os.path import join as pjoin

opts = Variables('build.py')

opts.Add(PathVariable('EV', 'Path to libev', '/usr/local'))

env = Environment(options=opts,
                  ENV = os.environ.copy(),
                  tools=['default'])

#TODO: convert this to a configure builder, so it gets cached
def read_version(prefix, path):
  version_re = re.compile("(.*)%s_VERSION_(?P<id>MAJOR|MINOR|PATCH)(\s+)(?P<num>\d)(.*)" % prefix)
  versions = {}
  fp = open(path, 'rb')
  for line in fp.readlines():
    m = version_re.match(line)
    if m:
      versions[m.group('id')] = int(m.group('num'))
  fp.close()
  return (versions['MAJOR'], versions['MINOR'], versions['PATCH'])

env['version_major'], env['version_minor'], env['version_patch'] = read_version('DL', 'src/dl_version.h')
env['version_string'] = "%d.%d.%d"  % (env['version_major'], env['version_minor'], env['version_patch'])

conf = Configure(env, custom_tests = {})

cc = conf.env.WhereIs('/Developer/usr/bin/clang')
if os.environ.has_key('CC'):
  cc = os.environ['CC']

if cc:
  conf.env['CC'] = cc

conf.env.AppendUnique(LIBPATH=pjoin(conf.env['EV'], 'lib'))
conf.env.AppendUnique(CPPPATH=pjoin(conf.env['EV'], 'include'))
if not conf.CheckLibWithHeader('ev', 'ev++.h', 'C++'):
  print 'Did not find libev in %s, exiting!' % (conf.env['EV'])
  Exit(1)

# this is needed on solaris because of its dumb library path issues
conf.env.AppendUnique(RPATH = conf.env.get('LIBPATH'))
env = conf.Finish()

Export("env")

dislocate = SConscript("src/SConscript")

targets = [dislocate]

env.Default(targets)
