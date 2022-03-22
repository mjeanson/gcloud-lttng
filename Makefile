# SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>
#
# SPDX-License-Identifier: MIT

.PHONY: pods provision push-images deploy undeploy deprovision

PODS = \
	pods/demo1.yaml

all: pods
pods: $(PODS)

# Basic templating of pods yaml files
%.yaml: %.yaml.in etc/conf.sh
	. etc/conf.sh && envsubst < $< > $@


provision:
	./scripts/provision.sh

push-images:
	./scripts/push-images.sh

deploy: $(PODS)
	./scripts/deploy.sh

undeploy: $(PODS)
	./scripts/undeploy.sh

deprovision:
	./scripts/deprovision.sh
