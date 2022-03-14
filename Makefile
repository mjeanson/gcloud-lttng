.PHONY: pods provision push-images deploy cleanup

PODS = \
	pods/demo1.yaml

all: pods
pods: $(PODS)

# Basic templating of pods yaml files
%.yaml: %.yaml.in etc/conf.sh
	. etc/conf.sh && envsubst < $< > $@


provision:
	./provision.sh

push-images:
	./push-images.sh

deploy: $(PODS)
	./deploy.sh

cleanup:
	./cleanup.sh
