terraform {
  backend "s3" {
    bucket         = "movement-sensor-tfstate"
    key            = "sensor-access/terraform.tfstate"
    region         = "us-east-1"
    dynamodb_table = "sensor-access-dev-tfstate-lock"
    encrypt        = true
  }
}


