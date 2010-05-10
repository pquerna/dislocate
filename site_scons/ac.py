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

import os
import hashlib


def CheckCurl(context, args):
  prog = context.env['CURL']
  context.Message("Checking %s %s ...." % (prog, args))
  m = hashlib.md5()
  m.update(args)
  
  output = context.sconf.confdir.File(os.path.basename(prog)+ '-'+ m.hexdigest() +'.out') 
  node = context.sconf.env.Command(output, prog, [ [ prog, args, ">", "${TARGET}"] ]) 
  ok = context.sconf.BuildNodes(node) 
  if ok: 
    outputStr = output.get_contents().strip()
    context.Result(" "+ outputStr)
    return (1, outputStr)
  else:
    context.Result("error running curl-config")
    return (0, "")

def CheckCurlPrefix(context):
  return CheckCurl(context, '--prefix')

def CheckCurlLibs(context):
  return CheckCurl(context, '--libs')


def CheckDpkgArch(context):
  args = "-qDEB_BUILD_ARCH"
  prog = context.env.WhereIs("dpkg-architecture")
  if not prog:
    context.Message("Error: `dpkg-architecture` not found. Install dpkg-dev?")
    return (0, "")
  context.Message("Checking %s %s ...." % (prog, args))
  output = context.sconf.confdir.File(os.path.basename(prog)+'.out') 
  node = context.sconf.env.Command(output, prog, [ [ prog, args, ">", "${TARGET}"] ]) 
  ok = context.sconf.BuildNodes(node) 
  if ok: 
    outputStr = output.get_contents().strip()
    context.Result(" "+ outputStr)
    return (1, outputStr)
  else:
    context.Result("error running dpkg-architecture")
    return (0, "")

