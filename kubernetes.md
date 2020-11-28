This uses kind as a kubernetes backend.


### Create cluster:
Create 3 nodes
`kind create cluster --config kind-config.yml`



### Create namespace

`kubectl create -f namespace.json`

`kubectl config view`

https://kubernetes.io/docs/tasks/administer-cluster/namespaces-walkthrough/

```
kubectl config set-context traffic --namespace=traffic \
  --cluster=kind-kind \
  --user=kind-kind
```

`kubectl config use-context traffic`

### What is LoadBalancer DNS address?
https://github.com/kubernetes/dns/blob/master/docs/specification.md

`rabbitmq.traffic.svc.cluster.local`
`<service-name>.<namespace>.svc.cluster.local`