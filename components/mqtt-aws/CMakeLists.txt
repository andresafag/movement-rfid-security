idf_component_register(SRCS "mqtt-aws.c"
                    INCLUDE_DIRS "include"
                    REQUIRES  esp_event
                    EMBED_TXTFILES "certs/aws-root-ca.pem"
                                   "certs/certificate.pem.crt"
                                   "certs/private.pem.key")
