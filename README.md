<!--
SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>

SPDX-License-Identifier: MIT
-->

Prerequisites
----------------

The following tools need to be installed and working:

  - docker
  - kubectl
  - google-cloud-cli

Getting started
---------------
Create a configuration file.

    cp etc/conf.sh.example etc/conf.sh
 
 Create the Gcloud cluster and the docker image repository.
 
    make provision

Build and push the images to the repository.

    make push-images

Deploy the pod to the cluster.
 
    make deploy

Monitor the pod status

    kubectl describe pod demo1
  
Destroy the cluster and image repository.

    make deprovision
