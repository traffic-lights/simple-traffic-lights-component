cluster: 
	time gcloud container clusters create rabbit-cluster \
           --release-channel rapid \
           --cluster-version 1.18.10-gke.2101 \
           --node-version 1.17.9-gke.1504 \
           --machine-type n1-standard-1 \
           --num-nodes 2 \
           --enable-shielded-nodes \
           --disk-type "pd-ssd" \
           --disk-size "10" \
           --enable-ip-alias \
           --enable-autoupgrade \
           --enable-autorepair \
           --max-surge-upgrade 2 \
           --max-unavailable-upgrade 0 \
           --metadata disable-legacy-endpoints=true \
           --no-enable-master-authorized-networks \
           --addons "HorizontalPodAutoscaling,HttpLoadBalancing"


brokers: 
	kubectl apply -f configmaps \
	&& kubectl apply -f roles \
	&& kubectl apply -f storage.yml \
	&& kubectl apply -f services \
	&& kubectl apply -f statefulset-new.yml 


servers: 
	kubectl apply -f deployment-2x2.yml

update-brokers: 
	yq merge statefulset.rabbitmq-upgrade statefulset-new.yml \
	| kubectl apply --filename -

credentials:
	kubectl create secret generic erlang-cookie \
	--dry-run=client --output=yaml --from-literal=value="my_erlang_cookie_val" > roles/secret.erlang-cookie.yml \
	&& kubectl create secret generic default-user \
	--dry-run=client --output=yaml --from-literal=value="default_user = kaszper" > roles/secret.default-user.yml \
	&& kubectl create secret generic default-pass \
	--dry-run=client --output=yaml --from-literal=value="default_pass = kaszper" > roles/secret.default-pass.yml
